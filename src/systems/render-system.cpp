#include "render-system.h"

#include "scomponents/graphics/camera.h"
#include "graphics/constant-buffer.h"

RenderSystem::RenderSystem(Context& context) : m_ctx(context) {
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::update() {

    m_ctx.registry.view<comp::CubeMesh, comp::Pipeline, comp::Position>()
    .each([&](met::entity entity, comp::CubeMesh& mesh, comp::Pipeline& pipeline, comp::Position& position) {

    });

}

