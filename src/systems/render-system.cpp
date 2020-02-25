#include "render-system.h"

#include <debug_break/debug_break.h>
#include <profiling/instrumentor.h>

#include "graphics/constant-buffer.h"
#include "graphics/gl-exception.h"
#include "maths/casting.h"


RenderSystem::RenderSystem() {
    m_tempTranslations.reserve(15);
    m_tempEntityIds.reserve(15);
}

RenderSystem::~RenderSystem() {}

void RenderSystem::update(const RenderCommand& rc, const ConstantBuffers& cbs, const Meshes& meshes, 
	const Pipelines& pips, RenderTargets& rtts, Materials& mats, Lights& lights, Camera& cam,
    const Inputs& inputs, const Hovered& hovered, const Viewport& viewport)
{
    PROFILE_SCOPE("RenderSystem update");

	{
        OGL_SCOPE("Update perFrame constant buffer");
		cb::perFrame cbData;
        const ConstantBuffer& perFrameCB = cbs.at(ConstantBufferIndex::PER_FRAME);

        // Set data
        cbData.cameraPos = cam.position();
        cbData.matViewProj =  cam.proj() * cam.view();
        if (inputs.isEnabled(InputAction::DEBUG)) {
            cbData.debug2 = 1;
        } else {
            cbData.debug2 = 0;
        }

        // Send data
		rc.updateConstantBuffer(perFrameCB, &cbData, sizeof(cb::perFrame));
        cam.m_hasToBeUpdated = false;
	}

    // Update per Material change constant buffer
	if (mats.hasToBeUpdated()) {
        OGL_SCOPE("Update perMatChange constant buffer");
        const ConstantBuffer& perMatChangeCB = cbs.at(ConstantBufferIndex::PER_MATERIAL_CHANGE);
        rc.updateConstantBuffer(perMatChangeCB, mats.data(), sizeof(cb::perMaterialChange) * mats.size());
        mats.m_hasToBeUpdated = false;
    }

    // Update per Light change constant buffer
	if (lights.hasToBeUpdated()) {
        OGL_SCOPE("Update perLightChange constant buffer & perShadow pass");

        // Directionnal lights
        const ConstantBuffer& perLightChangeCB = cbs.at(ConstantBufferIndex::PER_LIGHT_CHANGE);
		rc.updateConstantBuffer(perLightChangeCB, lights.directionalsData(), lights.directionalsByteWidth());

        // TODO other lights with buffer update offset

        lights.m_hasToBeUpdated = false;

        {
            cb::perLightChange::perShadowPass cbData;
            const ConstantBuffer& perShadowPassCB = cbs.at(ConstantBufferIndex::PER_SHADOW_PASS);

            // Set data
            const glm::vec3 lightDir = lights.directionals(0).direction;
            glm::mat4 lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -20.0f, 20.0f);
            glm::mat4 lightView = glm::lookAt(-lightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
            cbData.matViewProj_lightSpace = lightProj * lightView;

            // Send data
            rc.updateConstantBuffer(perShadowPassCB, &cbData, sizeof(cb::perLightChange::perShadowPass));
        }
	}

    
    unsigned int nbInstances = 0;

    // All cubes are using the same mesh and shaders
    // TODO use tag to only grab cubes
    /*
    view.each([&](met::entity entity, comp::Material& material, comp::Transform& transform) {
        nbInstances++;
        m_tempTranslations.push_back(transform.position);
        m_tempEntityIds.push_back(voxmt::intToNormColor(entity));
        m_tempMaterialIds.push_back(material.sIndex);

        if (nbInstances >= view.size()) {
            // Update instance buffers
            for (auto& buffer : meshes.m_cube.vb.buffers) {
                switch (buffer.type) {
                case AttributeBufferType::PER_INSTANCE_TRANSLATION: {
                    OGL_SCOPE("Update perInstanceTranslation attribute buffer");
                    rc.updateAttributeBufferAnySize(buffer, m_tempTranslations.data(), sizeof(glm::vec3) * nbInstances);
                    m_tempTranslations.clear();
                    break;
                }

                case AttributeBufferType::PER_INSTANCE_ENTITY_ID: {
                    OGL_SCOPE("Update perInstanceEntityId attribute buffer");
                    rc.updateAttributeBufferAnySize(buffer, m_tempEntityIds.data(), sizeof(glm::vec3) * nbInstances);
                    m_tempEntityIds.clear();
                    break;
                }

                case AttributeBufferType::PER_INSTANCE_MATERIAL: {
                    OGL_SCOPE("Update perInstanceMaterial attribute buffer");
                    rc.updateAttributeBufferAnySize(buffer, m_tempMaterialIds.data(), sizeof(unsigned int) * nbInstances);
                    m_tempMaterialIds.clear();
                    break;
                }

                default: break;
                }
            }
        }
    });
    */


    {
        OGL_SCOPE("Geometry pass");
        rc.enableDepthTest();
        rc.bindVertexBuffer(meshes.cube().vb);
        rc.bindIndexBuffer(meshes.cube().ib);
        rc.bindRenderTarget(rtts.at(RenderTargetIndex::RTT_GEOMETRY));
        rc.clear();
        rc.bindPipeline(pips.at(PipelineIndex::PIP_GEOMETRY));
        rc.drawIndexedInstances(meshes.cube().ib.count, meshes.cube().ib.type, nbInstances);
        const glm::ivec2 pixelToRead = glm::ivec2(inputs.mousePos().x, viewport.size().y - inputs.mousePos().y);
        rc.prepareReadPixelBuffer(rtts.m_rts.at(static_cast<unsigned int>(RenderTargetIndex::RTT_GEOMETRY)).pixelBuffer, pixelToRead);
    }

    {
        OGL_SCOPE("Shadow map pass");
        rc.bindRenderTarget(rtts.at(RenderTargetIndex::RTT_SHADOW_MAP));
        rc.clear();
        rc.bindPipeline(pips.at(PipelineIndex::PIP_SHADOW_MAP));
        rc.drawIndexedInstances(meshes.cube().ib.count, meshes.cube().ib.type, nbInstances);
    }
    
    {
        OGL_SCOPE("Lighting pass");
        rc.bindVertexBuffer(meshes.plane().vb);
        rc.bindIndexBuffer(meshes.plane().ib);
        rc.bindRenderTarget(rtts.at(RenderTargetIndex::RTT_FINAL));
        rc.clear();
        rc.bindPipeline(pips.at(PipelineIndex::PIP_LIGHTING));
        std::vector<unsigned int> textureIds = rtts.at(RenderTargetIndex::RTT_GEOMETRY).textureIds;
        textureIds.push_back(rtts.at(RenderTargetIndex::RTT_SHADOW_MAP).textureIds.at(0));
        rc.bindTextureIds(textureIds);
        rc.drawIndexed(meshes.plane().ib.count, meshes.plane().ib.type);
    }

    {
        OGL_SCOPE("Grid pass");
        rc.bindVertexBuffer(meshes.invertCube().vb);
        rc.bindIndexBuffer(meshes.invertCube().ib);
        rc.bindPipeline(pips.at(PipelineIndex::PIP_GRID));
        rc.drawIndexed(meshes.invertCube().ib.count, meshes.invertCube().ib.type);
    }

    // Debug draw pass
    /*
    {
        m_ctx.ddraw.updateBuffer();
        rc.bindVertexBuffer(ddrawVb);
        rc.bindPipeline(pips.at(scomp::PipelineIndex::PIP_DDRAW));
        rc.drawLines(m_ctx.ddraw.getCount());
    }
    */

    
    {
        OGL_SCOPE("GUI pass");
        rc.disableDepthTest();
        rc.bindPipeline(pips.at(PipelineIndex::PIP_GUI));

        {
            OGL_SCOPE("Selection plane");
            rc.bindVertexBuffer(meshes.plane().vb);
            rc.bindIndexBuffer(meshes.plane().ib);
            updateCBperNiMesh_facePlane(rc, cbs, hovered);
            rc.drawIndexed(meshes.plane().ib.count, meshes.plane().ib.type);
        }
    }
}

void RenderSystem::updateCBperNiMesh(const RenderCommand& rc, const ConstantBuffers& cbs, glm::vec3 translation, float scale, glm::vec3 albedo) {
    cb::perNiMesh cbData;
    const ConstantBuffer& perNiMeshCB = cbs.at(ConstantBufferIndex::PER_NI_MESH);

    cbData.matWorld = glm::translate(glm::mat4(1), translation);
    cbData.matWorld = glm::scale(cbData.matWorld, glm::vec3(scale));
    cbData.albedo = albedo;

    rc.updateConstantBuffer(perNiMeshCB, &cbData, sizeof(cb::perNiMesh));
}

void RenderSystem::updateCBperNiMesh_facePlane(const RenderCommand& rc, const ConstantBuffers& cbs, const Hovered& hovered) {
    cb::perNiMesh cbData;
    const ConstantBuffer& perNiMeshCB = cbs.at(ConstantBufferIndex::PER_NI_MESH);

    if (hovered.exist()) {
        glm::vec3 pos = hovered.position();
        pos += 0.5f;

        switch (hovered.face()) {
            case Face::FRONT:
                (hovered.isCube()) ? pos.z -= 0.5f : pos.z += 0.5f;
                cbData.matWorld = glm::translate(glm::mat4(1), pos); 
                break;

            case Face::BACK:
                (hovered.isCube()) ? pos.z += 0.5f : pos.z -= 0.5f;
                cbData.matWorld = glm::translate(glm::mat4(1), pos);
                cbData.matWorld = glm::rotate(cbData.matWorld, glm::pi<float>(), glm::vec3(0, 1, 0));
                break;

            case Face::RIGHT:
                (hovered.isCube()) ? pos.x += 0.5f : pos.x -= 0.5f; 
                cbData.matWorld = glm::translate(glm::mat4(1), pos);
                cbData.matWorld = glm::rotate(cbData.matWorld, -glm::half_pi<float>(), glm::vec3(0, 1, 0));
                break;

            case Face::LEFT:
                (hovered.isCube()) ? pos.x -= 0.5f : pos.x += 0.5f; 
                cbData.matWorld = glm::translate(glm::mat4(1), pos);
                cbData.matWorld = glm::rotate(cbData.matWorld, glm::half_pi<float>(), glm::vec3(0, 1, 0));
                break;

            case Face::TOP:
                (hovered.isCube()) ? pos.y += 0.5f : pos.y -= 0.5f; 
                cbData.matWorld = glm::translate(glm::mat4(1), pos);
                cbData.matWorld = glm::rotate(cbData.matWorld, glm::half_pi<float>(), glm::vec3(1, 0, 0));
                break;
            
            case Face::BOTTOM:
                (hovered.isCube()) ? pos.y -= 0.5f : pos.y += 0.5f;
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

    rc.updateConstantBuffer(perNiMeshCB, &cbData, sizeof(cb::perNiMesh));
}
