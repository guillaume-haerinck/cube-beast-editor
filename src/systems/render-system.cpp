#include "render-system.h"

#include <debug_break/debug_break.h>

#include "graphics/constant-buffer.h"
#include "components/graphics/material.h"
#include "components/physics/transform.h"
#include "maths/casting.h"

// Temp
#ifdef __EMSCRIPTEN__
	#include <GLES3/gl3.h>
#else
	#include <glad/gles2.h>
#endif
#include "graphics/gl-exception.h"

RenderSystem::RenderSystem(Context& context, SingletonComponents& scomps) : m_ctx(context), m_scomps(scomps) {
    m_tempTranslations.reserve(15);
    m_tempEntityIds.reserve(15);
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::update() {
    // Update per frame constant buffer
	{
		cb::perFrame cbData;
        const ConstantBuffer& perFrameCB = m_scomps.constantBuffers.at(ConstantBufferIndex::PER_FRAME);

        // Set data
        cbData.cameraPos = m_scomps.camera.position();
        cbData.matViewProj =  m_scomps.camera.proj() * m_scomps.camera.view();

        // Send data
		m_ctx.rcommand.updateConstantBuffer(perFrameCB, &cbData);
	}

    auto view = m_ctx.registry.view<comp::Material, comp::Transform>();
    unsigned int nbInstances = 0;

    // All cubes are using the same mesh and shaders
    // TODO use tag to only grab cubes
    view.each([&](met::entity entity, comp::Material& material, comp::Transform& transform) {
        nbInstances++;
        m_tempTranslations.push_back(transform.position);
        m_tempEntityIds.push_back(voxmt::intToNormColor(entity));
        m_tempMaterialIds.push_back(nbInstances - 1);

        if (nbInstances >= view.size()) {
            // Update instance buffers
            for (auto& buffer : m_scomps.meshes.m_cube.vb.buffers) {
                switch (buffer.type) {
                case AttributeBufferType::PER_INSTANCE_TRANSLATION:
                    m_ctx.rcommand.updateAttributeBufferAnySize(buffer, m_tempTranslations.data(), sizeof(glm::vec3) * nbInstances);
                    m_tempTranslations.clear();
                    break;

                case AttributeBufferType::PER_INSTANCE_ENTITY_ID:
                    m_ctx.rcommand.updateAttributeBufferAnySize(buffer, m_tempEntityIds.data(), sizeof(glm::vec3) * nbInstances);
                    m_tempEntityIds.clear();
                    break;

                case AttributeBufferType::PER_INSTANCE_MATERIAL:
                    m_ctx.rcommand.updateAttributeBufferAnySize(buffer, m_tempMaterialIds.data(), sizeof(unsigned int) * nbInstances);
                    m_tempMaterialIds.clear();
                    break;

                default: break;
                }
            }
        }
    });

    // Geometry pass
    {
        m_ctx.rcommand.enableDepthTest();
        m_ctx.rcommand.bindVertexBuffer(m_scomps.meshes.cube().vb);
        m_ctx.rcommand.bindIndexBuffer(m_scomps.meshes.cube().ib);
        m_ctx.rcommand.bindRenderTarget(m_scomps.renderTargets.at(RenderTargetIndex::RTT_GEOMETRY));
        m_ctx.rcommand.clear();
        m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(PipelineIndex::PIP_GEOMETRY));
        m_ctx.rcommand.drawIndexedInstances(m_scomps.meshes.cube().ib.count, m_scomps.meshes.cube().ib.type, nbInstances);
    }
    
    // Lighting pass
    {
        m_ctx.rcommand.bindVertexBuffer(m_scomps.meshes.plane().vb);
        m_ctx.rcommand.bindIndexBuffer(m_scomps.meshes.plane().ib);
        m_ctx.rcommand.bindRenderTarget(m_scomps.renderTargets.at(RenderTargetIndex::RTT_FINAL));
        m_ctx.rcommand.clear();
        m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(PipelineIndex::PIP_LIGHTING));
        m_ctx.rcommand.bindTextureIds(m_scomps.renderTargets.at(RenderTargetIndex::RTT_GEOMETRY).textureIds);
        m_ctx.rcommand.drawIndexed(m_scomps.meshes.plane().ib.count, m_scomps.meshes.plane().ib.type);
    }

    // Grid pass
    {
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

    // Gui pass
    {
        m_ctx.rcommand.disableDepthTest();
        m_ctx.rcommand.bindVertexBuffer(m_scomps.meshes.plane().vb);
        m_ctx.rcommand.bindIndexBuffer(m_scomps.meshes.plane().ib);
        m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(PipelineIndex::PIP_GUI));

        // Update per non-instanced mesh constant buffer
        {
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

            m_ctx.rcommand.updateConstantBuffer(perNiMeshCB, &cbData);
        }

        m_ctx.rcommand.drawIndexed(m_scomps.meshes.plane().ib.count, m_scomps.meshes.plane().ib.type);
    }
}
