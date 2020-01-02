#include "render-system.h"

#include <debug_break/debug_break.h>
#include <profiling/instrumentor.h>

#include "graphics/constant-buffer.h"
#include "graphics/gl-exception.h"
#include "components/graphics/material.h"
#include "components/physics/transform.h"
#include "maths/casting.h"

RenderSystem::RenderSystem(Context& context, SingletonComponents& scomps) : m_ctx(context), m_scomps(scomps) {
    m_tempTranslations.reserve(15);
    m_tempEntityIds.reserve(15);
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::update() {
    PROFILE_SCOPE("RenderSystem update");

	{
        OGL_SCOPE("Update perFrame constant buffer");
		cb::perFrame cbData;
        const ConstantBuffer& perFrameCB = m_scomps.constantBuffers.at(ConstantBufferIndex::PER_FRAME);

        // Set data
        cbData.cameraPos = m_scomps.camera.position();
        cbData.matViewProj =  m_scomps.camera.proj() * m_scomps.camera.view();
        cbData.debug2 = 0;

        // Send data
		m_ctx.rcommand.updateConstantBuffer(perFrameCB, &cbData, sizeof(cb::perFrame));
        m_scomps.camera.m_hasToBeUpdated = false;
	}

    // Update per Material change constant buffer
	if (m_scomps.materials.hasToBeUpdated()) {
        OGL_SCOPE("Update perMatChange constant buffer");
        const ConstantBuffer& perMatChangeCB = m_scomps.constantBuffers.at(ConstantBufferIndex::PER_MATERIAL_CHANGE);
        m_ctx.rcommand.updateConstantBuffer(perMatChangeCB, m_scomps.materials.data(), sizeof(cb::perMaterialChange) * m_scomps.materials.size());
        m_scomps.materials.m_hasToBeUpdated = false;
    }

    // Update per Light change constant buffer
	if (m_scomps.lights.hasToBeUpdated()) {
        OGL_SCOPE("Update perLightChange constant buffer & perShadow pass");

        // Directionnal lights
        const ConstantBuffer& perLightChangeCB = m_scomps.constantBuffers.at(ConstantBufferIndex::PER_LIGHT_CHANGE);
		m_ctx.rcommand.updateConstantBuffer(perLightChangeCB, m_scomps.lights.directionalsData(), m_scomps.lights.directionalsByteWidth());

        // TODO other lights with buffer update offset

        m_scomps.lights.m_hasToBeUpdated = false;

        {
            cb::perLightChange::perShadowPass cbData;
            const ConstantBuffer& perShadowPassCB = m_scomps.constantBuffers.at(ConstantBufferIndex::PER_SHADOW_PASS);

            // Set data
            const glm::vec3 lightDir = m_scomps.lights.directionals(0).direction;
            glm::mat4 lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -20.0f, 20.0f);
            glm::mat4 lightView = glm::lookAt(-lightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
            cbData.matViewProj_lightSpace = lightProj * lightView;

            // Send data
            m_ctx.rcommand.updateConstantBuffer(perShadowPassCB, &cbData, sizeof(cb::perLightChange::perShadowPass));
        }
	}

    auto view = m_ctx.registry.view<comp::Material, comp::Transform>();
    unsigned int nbInstances = 0;

    // All cubes are using the same mesh and shaders
    // TODO use tag to only grab cubes
    view.each([&](met::entity entity, comp::Material& material, comp::Transform& transform) {
        nbInstances++;
        m_tempTranslations.push_back(transform.position);
        m_tempEntityIds.push_back(voxmt::intToNormColor(entity));
        m_tempMaterialIds.push_back(material.sIndex);

        if (nbInstances >= view.size()) {
            // Update instance buffers
            for (auto& buffer : m_scomps.meshes.m_cube.vb.buffers) {
                switch (buffer.type) {
                case AttributeBufferType::PER_INSTANCE_TRANSLATION: {
                    OGL_SCOPE("Update perInstanceTranslation attribute buffer");
                    m_ctx.rcommand.updateAttributeBufferAnySize(buffer, m_tempTranslations.data(), sizeof(glm::vec3) * nbInstances);
                    m_tempTranslations.clear();
                    break;
                }

                case AttributeBufferType::PER_INSTANCE_ENTITY_ID: {
                    OGL_SCOPE("Update perInstanceEntityId attribute buffer");
                    m_ctx.rcommand.updateAttributeBufferAnySize(buffer, m_tempEntityIds.data(), sizeof(glm::vec3) * nbInstances);
                    m_tempEntityIds.clear();
                    break;
                }

                case AttributeBufferType::PER_INSTANCE_MATERIAL: {
                    OGL_SCOPE("Update perInstanceMaterial attribute buffer");
                    m_ctx.rcommand.updateAttributeBufferAnySize(buffer, m_tempMaterialIds.data(), sizeof(unsigned int) * nbInstances);
                    m_tempMaterialIds.clear();
                    break;
                }

                default: break;
                }
            }
        }
    });


    {
        OGL_SCOPE("Geometry pass");
        m_ctx.rcommand.enableDepthTest();
        m_ctx.rcommand.bindVertexBuffer(m_scomps.meshes.cube().vb);
        m_ctx.rcommand.bindIndexBuffer(m_scomps.meshes.cube().ib);
        m_ctx.rcommand.bindRenderTarget(m_scomps.renderTargets.at(RenderTargetIndex::RTT_GEOMETRY));
        m_ctx.rcommand.clear();
        m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(PipelineIndex::PIP_GEOMETRY));
        m_ctx.rcommand.drawIndexedInstances(m_scomps.meshes.cube().ib.count, m_scomps.meshes.cube().ib.type, nbInstances);
        const glm::ivec2 pixelToRead = glm::ivec2(m_scomps.inputs.mousePos().x, m_scomps.viewport.size().y - m_scomps.inputs.mousePos().y);
        m_ctx.rcommand.prepareReadPixelBuffer(m_scomps.renderTargets.m_rts.at(static_cast<unsigned int>(RenderTargetIndex::RTT_GEOMETRY)).pixelBuffer, pixelToRead);
    }

    {
        OGL_SCOPE("Shadow map pass");
        m_ctx.rcommand.bindRenderTarget(m_scomps.renderTargets.at(RenderTargetIndex::RTT_SHADOW_MAP));
        m_ctx.rcommand.clear();
        m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(PipelineIndex::PIP_SHADOW_MAP));
        m_ctx.rcommand.drawIndexedInstances(m_scomps.meshes.cube().ib.count, m_scomps.meshes.cube().ib.type, nbInstances);
    }
    
    {
        OGL_SCOPE("Lighting pass");
        m_ctx.rcommand.bindVertexBuffer(m_scomps.meshes.plane().vb);
        m_ctx.rcommand.bindIndexBuffer(m_scomps.meshes.plane().ib);
        m_ctx.rcommand.bindRenderTarget(m_scomps.renderTargets.at(RenderTargetIndex::RTT_FINAL));
        m_ctx.rcommand.clear();
        m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(PipelineIndex::PIP_LIGHTING));
        std::vector<unsigned int> textureIds = m_scomps.renderTargets.at(RenderTargetIndex::RTT_GEOMETRY).textureIds;
        textureIds.push_back(m_scomps.renderTargets.at(RenderTargetIndex::RTT_SHADOW_MAP).textureIds.at(0));
        m_ctx.rcommand.bindTextureIds(textureIds);
        m_ctx.rcommand.drawIndexed(m_scomps.meshes.plane().ib.count, m_scomps.meshes.plane().ib.type);
    }

    {
        OGL_SCOPE("Grid pass");
        m_ctx.rcommand.bindVertexBuffer(m_scomps.meshes.invertCube().vb);
        m_ctx.rcommand.bindIndexBuffer(m_scomps.meshes.invertCube().ib);
        m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(PipelineIndex::PIP_GRID));
        m_ctx.rcommand.drawIndexed(m_scomps.meshes.invertCube().ib.count, m_scomps.meshes.invertCube().ib.type);
    }

    // Debug draw pass
    /*
    {
        m_ctx.ddraw.updateBuffer();
        m_ctx.rcommand.bindVertexBuffer(m_scomps.ddrawVb);
        m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(scomp::PipelineIndex::PIP_DDRAW));
        m_ctx.rcommand.drawLines(m_ctx.ddraw.getCount());
    }
    */

    
    {
        OGL_SCOPE("GUI pass");
        m_ctx.rcommand.disableDepthTest();
        m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(PipelineIndex::PIP_GUI));

        {
            OGL_SCOPE("Selection plane");
            m_ctx.rcommand.bindVertexBuffer(m_scomps.meshes.plane().vb);
            m_ctx.rcommand.bindIndexBuffer(m_scomps.meshes.plane().ib);
            updateCBperNiMesh_facePlane();
            m_ctx.rcommand.drawIndexed(m_scomps.meshes.plane().ib.count, m_scomps.meshes.plane().ib.type);
        }
    }
}

void RenderSystem::updateCBperNiMesh(glm::vec3 translation, float scale, glm::vec3 albedo) {
    cb::perNiMesh cbData;
    const ConstantBuffer& perNiMeshCB = m_scomps.constantBuffers.at(ConstantBufferIndex::PER_NI_MESH);

    cbData.matWorld = glm::translate(glm::mat4(1), translation);
    cbData.matWorld = glm::scale(cbData.matWorld, glm::vec3(scale));
    cbData.albedo = albedo;

    m_ctx.rcommand.updateConstantBuffer(perNiMeshCB, &cbData, sizeof(cb::perNiMesh));
}

void RenderSystem::updateCBperNiMesh_facePlane() {
    cb::perNiMesh cbData;
    const ConstantBuffer& perNiMeshCB = m_scomps.constantBuffers.at(ConstantBufferIndex::PER_NI_MESH);

    if (m_scomps.hovered.exist()) {
        glm::vec3 pos = m_scomps.hovered.position();
        pos += 0.5f;

        switch (m_scomps.hovered.face()) {
            case Face::FRONT:
                (m_scomps.hovered.isCube()) ? pos.z -= 0.5f : pos.z += 0.5f;
                cbData.matWorld = glm::translate(glm::mat4(1), pos); 
                break;

            case Face::BACK:
                (m_scomps.hovered.isCube()) ? pos.z += 0.5f : pos.z -= 0.5f;
                cbData.matWorld = glm::translate(glm::mat4(1), pos);
                cbData.matWorld = glm::rotate(cbData.matWorld, glm::pi<float>(), glm::vec3(0, 1, 0));
                break;

            case Face::RIGHT:
                (m_scomps.hovered.isCube()) ? pos.x += 0.5f : pos.x -= 0.5f; 
                cbData.matWorld = glm::translate(glm::mat4(1), pos);
                cbData.matWorld = glm::rotate(cbData.matWorld, -glm::half_pi<float>(), glm::vec3(0, 1, 0));
                break;

            case Face::LEFT:
                (m_scomps.hovered.isCube()) ? pos.x -= 0.5f : pos.x += 0.5f; 
                cbData.matWorld = glm::translate(glm::mat4(1), pos);
                cbData.matWorld = glm::rotate(cbData.matWorld, glm::half_pi<float>(), glm::vec3(0, 1, 0));
                break;

            case Face::TOP:
                (m_scomps.hovered.isCube()) ? pos.y += 0.5f : pos.y -= 0.5f; 
                cbData.matWorld = glm::translate(glm::mat4(1), pos);
                cbData.matWorld = glm::rotate(cbData.matWorld, glm::half_pi<float>(), glm::vec3(1, 0, 0));
                break;
            
            case Face::BOTTOM:
                (m_scomps.hovered.isCube()) ? pos.y -= 0.5f : pos.y += 0.5f;
                cbData.matWorld = glm::translate(glm::mat4(1), pos);
                cbData.matWorld = glm::rotate(cbData.matWorld, -glm::half_pi<float>(), glm::vec3(1, 0, 0));
                break;

            case Face::NONE:
                cbData.matWorld = glm::scale(glm::mat4(1), glm::vec3(0));
                break;
            
            default:
                debug_break();
                assert(false && "Unknown hovered face");
                cbData.matWorld = glm::scale(glm::mat4(1), glm::vec3(0));
        }
    } else {
        cbData.matWorld = glm::scale(glm::mat4(1), glm::vec3(0));
    }
    cbData.albedo = glm::vec3(1, 0.5, 0.5);

    m_ctx.rcommand.updateConstantBuffer(perNiMeshCB, &cbData, sizeof(cb::perNiMesh));
}
