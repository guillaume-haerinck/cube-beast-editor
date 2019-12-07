#include "render-system.h"

#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <glad/gles2.h>
#endif

#include "scomponents/graphics/camera.h"
#include "graphics/constant-buffer.h"
#include "components/graphics/material.h"
#include "components/graphics/pipeline.h"

RenderSystem::RenderSystem(Context& context, SingletonComponents& scomps) : m_ctx(context), m_scomps(scomps) {
    m_tempTranslations.reserve(15);
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

    auto view = m_ctx.registry.view<comp::Material, comp::Pipeline, comp::Transform>();
    unsigned int nbInstances = 0;

    view.each([&](met::entity entity, comp::Material& material, comp::Pipeline& pipeline, comp::Transform& transform) {
        nbInstances++;
        m_tempTranslations.push_back(transform.position);

        if (nbInstances >= view.size()) {
            // TODO use enum or something else to identify translation buffer. Plus check if it has to be updated
            m_ctx.rcommand.updateAttributeBuffer(m_scomps.cubeMesh.vb.buffers.at(2), m_tempTranslations.data(), sizeof(glm::vec3) * nbInstances);
            m_tempTranslations.clear();

            m_ctx.rcommand.bindVertexBuffer(m_scomps.cubeMesh.vb);
            m_ctx.rcommand.bindIndexBuffer(m_scomps.cubeMesh.ib);
            m_ctx.rcommand.bindPipeline(pipeline);

            m_ctx.rcommand.drawIndexedInstances(m_scomps.cubeMesh.ib.count, m_scomps.cubeMesh.ib.type, nbInstances);
        }
    });
}
