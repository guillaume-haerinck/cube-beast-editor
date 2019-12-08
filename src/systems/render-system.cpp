#include "render-system.h"

#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <glad/gles2.h>
#endif

#include "graphics/constant-buffer.h"
#include "components/graphics/material.h"

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

    // All entities are using the same mesh and shaders
    view.each([&](met::entity entity, comp::Material& material, comp::Transform& transform) {
        nbInstances++;
        m_tempTranslations.push_back(transform.position);
        m_tempEntityIds.push_back(entity);

        if (nbInstances >= view.size()) {
            // Update instance buffers
            for (auto buffer : m_scomps.cubeMesh.vb.buffers) {
                switch (buffer.type) {
                case scomp::AttributeBufferType::PER_INSTANCE_TRANSLATION:
                    m_ctx.rcommand.updateAttributeBuffer(buffer, m_tempTranslations.data(), sizeof(glm::vec3) * nbInstances);
                    m_tempTranslations.clear();
                    break;

                case scomp::AttributeBufferType::PER_INSTANCE_ENTITY_ID:
                    m_ctx.rcommand.updateAttributeBuffer(buffer, m_tempEntityIds.data(), sizeof(met::entity) * nbInstances);
                    m_tempEntityIds.clear();
                    break;

                default: break;
                }
            }

            m_ctx.rcommand.bindVertexBuffer(m_scomps.cubeMesh.vb);
            m_ctx.rcommand.bindIndexBuffer(m_scomps.cubeMesh.ib);

            // Picking pass
            m_ctx.rcommand.bindRenderTargets(m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_PICKING));
            m_ctx.rcommand.clear();
            m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(scomp::PipelineIndex::PIP_PICKING));
            m_ctx.rcommand.drawIndexedInstances(m_scomps.cubeMesh.ib.count, m_scomps.cubeMesh.ib.type, nbInstances);

            // Temp
            unsigned int pixel;
            glReadBuffer(GL_COLOR_ATTACHMENT0);
            glReadPixels(250, 250, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixel);
            
            // Basic pass
            // TODO use picking pass and apply its texture on a quad to prevent multiple pass. Must use an ubershader
            m_ctx.rcommand.unbindRenderTargets();
            m_ctx.rcommand.clear();
            m_ctx.rcommand.bindPipeline(m_scomps.pipelines.at(scomp::PipelineIndex::PIP_BASIC));
            m_ctx.rcommand.drawIndexedInstances(m_scomps.cubeMesh.ib.count, m_scomps.cubeMesh.ib.type, nbInstances);
        }
    });
}
