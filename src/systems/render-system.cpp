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
        scomp::ConstantBuffer& perFrameCB = m_scomps.constantBuffers.at(scomp::ConstantBufferIndex::PER_FRAME);

        // Set data
        cbData.cameraPos = m_scomps.camera.position;
        cbData.matViewProj =  m_scomps.camera.proj * m_scomps.camera.view;

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

        if (nbInstances >= view.size()) {
            // Update instance buffers
            for (auto buffer : m_scomps.cubeMesh.vb.buffers) {
                switch (buffer.type) {
                case scomp::AttributeBufferType::PER_INSTANCE_TRANSLATION:
                    m_ctx.rcommand.updateAttributeBuffer(buffer, m_tempTranslations.data(), sizeof(glm::vec3) * nbInstances);
                    m_tempTranslations.clear();
                    break;

                case scomp::AttributeBufferType::PER_INSTANCE_ENTITY_ID:
                    m_ctx.rcommand.updateAttributeBuffer(buffer, m_tempEntityIds.data(), sizeof(glm::vec3) * nbInstances);
                    m_tempEntityIds.clear();
                    break;

                default: break;
                }
            }
        }
    });

    // Geometry pass
    {
        m_ctx.rcommand.enableDepthTest();
        m_ctx.rcommand.bindVertexBuffer(m_scomps.cubeMesh.vb);
        m_ctx.rcommand.bindIndexBuffer(m_scomps.cubeMesh.ib);
        m_ctx.rcommand.bindRenderTargets(m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_GEOMETRY));
        m_ctx.rcommand.clear();
        m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(scomp::PipelineIndex::PIP_GEOMETRY));
        m_ctx.rcommand.drawIndexedInstances(m_scomps.cubeMesh.ib.count, m_scomps.cubeMesh.ib.type, nbInstances);
    }
    
    // Lighting pass
    {
        m_ctx.rcommand.bindVertexBuffer(m_scomps.planeMesh.vb);
        m_ctx.rcommand.bindIndexBuffer(m_scomps.planeMesh.ib);
        m_ctx.rcommand.unbindRenderTargets();
        m_ctx.rcommand.clear();
        m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(scomp::PipelineIndex::PIP_LIGHTING));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_GEOMETRY).textureIds.at(0));

        m_ctx.rcommand.drawIndexed(m_scomps.planeMesh.ib.count, m_scomps.planeMesh.ib.type);
    }

    // Grid pass
    {
        m_ctx.rcommand.bindVertexBuffer(m_scomps.invertCubeMesh.vb);
        m_ctx.rcommand.bindIndexBuffer(m_scomps.invertCubeMesh.ib);
        m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(scomp::PipelineIndex::PIP_GRID));
        m_ctx.rcommand.drawIndexed(m_scomps.invertCubeMesh.ib.count, m_scomps.invertCubeMesh.ib.type);
    }

    // Gui pass
    {
        m_ctx.rcommand.disableDepthTest();
        m_ctx.rcommand.bindVertexBuffer(m_scomps.planeMesh.vb);
        m_ctx.rcommand.bindIndexBuffer(m_scomps.planeMesh.ib);
        m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(scomp::PipelineIndex::PIP_GUI));

        // Update per non-instanced mesh constant buffer
        {
            cb::perNiMesh cbData;
            scomp::ConstantBuffer& perNiMeshCB = m_scomps.constantBuffers.at(scomp::ConstantBufferIndex::PER_NI_MESH);

            if (m_scomps.hoveredCube.id != met::null_entity) {
                comp::Transform trans = m_ctx.registry.get<comp::Transform>(m_scomps.hoveredCube.id);
                glm::vec3 pos = static_cast<glm::vec3>(trans.position);

                switch (m_scomps.hoveredCube.face) {
                    case scomp::Face::FRONT:
                        cbData.matWorld = glm::translate(glm::mat4(1), pos); 
                        break;

                    case scomp::Face::BACK:
                        cbData.matWorld = glm::translate(glm::mat4(1), pos);
                        cbData.matWorld = glm::rotate(cbData.matWorld, glm::pi<float>(), glm::vec3(0, 1, 0));
                        break;

                    case scomp::Face::RIGHT:
                        cbData.matWorld = glm::translate(glm::mat4(1), pos);
                        cbData.matWorld = glm::rotate(cbData.matWorld, -glm::half_pi<float>(), glm::vec3(0, 1, 0));
                        break;

                    case scomp::Face::LEFT:
                        cbData.matWorld = glm::translate(glm::mat4(1), pos);
                        cbData.matWorld = glm::rotate(cbData.matWorld, glm::half_pi<float>(), glm::vec3(0, 1, 0));
                        break;

                    case scomp::Face::TOP:
                        cbData.matWorld = glm::translate(glm::mat4(1), pos);
                        cbData.matWorld = glm::rotate(cbData.matWorld, glm::half_pi<float>(), glm::vec3(1, 0, 0));
                        break;
                    
                    case scomp::Face::BOTTOM:
                        cbData.matWorld = glm::translate(glm::mat4(1), pos);
                        cbData.matWorld = glm::rotate(cbData.matWorld, -glm::half_pi<float>(), glm::vec3(1, 0, 0));
                        break;
                    
                    default:
                        debug_break();
                        assert(false && "Unknown hovered face");
                        cbData.matWorld = glm::mat4(1);
                }
            } else {
                cbData.matWorld = glm::scale(glm::mat4(1), glm::vec3(0));
            }

            // TEMP
            cbData.matWorld = glm::scale(glm::mat4(1), glm::vec3(0));
            m_ctx.rcommand.updateConstantBuffer(perNiMeshCB, &cbData);
        }

        m_ctx.rcommand.drawIndexed(m_scomps.planeMesh.ib.count, m_scomps.planeMesh.ib.type);
        glEnable(GL_DEPTH_TEST);
    }

    // Debug draw pass
    {
        m_ctx.ddraw.updateBuffer();
        m_ctx.rcommand.bindVertexBuffer(m_scomps.ddrawVb);
        m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(scomp::PipelineIndex::PIP_DDRAW));
        m_ctx.rcommand.drawLines(m_ctx.ddraw.getCount());
    }
}
