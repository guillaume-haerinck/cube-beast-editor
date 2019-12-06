#include "render-system.h"

#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <glad/gles2.h>
#endif

#include "scomponents/graphics/camera.h"
#include "graphics/constant-buffer.h"
#include "components/graphics/material.h"
#include "components/physics/position.h"
#include "components/graphics/pipeline.h"

RenderSystem::RenderSystem(Context& context, const SingletonComponents& scomps) : m_ctx(context), m_scomps(scomps) {}

RenderSystem::~RenderSystem() {
}

void RenderSystem::update() {
    /*
    m_ctx.registry.view<comp::Material, comp::Pipeline, comp::Position>()
    .each([&](met::entity entity, comp::Material& material, comp::Pipeline& pipeline, comp::Position& position) {
        // m_ctx.rcommand.bindVertexBuffer(vb); // TODO use scomp
        m_ctx.rcommand.bindPipeline(pipeline);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    });
    */
}
