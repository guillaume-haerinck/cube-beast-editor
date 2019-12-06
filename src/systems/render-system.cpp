#include "render-system.h"

#include "scomponents/graphics/camera.h"
#include "graphics/constant-buffer.h"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/gles2.h>
#endif

RenderSystem::RenderSystem(Context& context) : m_ctx(context) {
    // TEMP TEST

    // Vertex buffer
    VertexInputDescription inputDescription = {
        { ShaderDataType::Float2, "Position" }
    };

    float positions[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        0.0f,  1.0f
    };
    comp::AttributeBuffer positionBuffer = m_ctx.rcommand.createAttributeBuffer(&positions, std::size(positions), sizeof(float));
    comp::VertexBuffer vertexBuffer = m_ctx.rcommand.createVertexBuffer(inputDescription, &positionBuffer);

    // Pipeline
    auto vs = m_ctx.rcommand.createVertexShader("res/shaders/basics/basic.vert");
    auto fs = m_ctx.rcommand.createFragmentShader("res/shaders/basics/basic.frag");
    comp::Pipeline pipeline = m_ctx.rcommand.createPipeline(vs, fs);

    // Position
    comp::Position worldPos = {};

    // Assign data to entity
    auto entity = m_ctx.registry.create();
    m_ctx.registry.assign<comp::VertexBuffer>(entity, vertexBuffer);
    m_ctx.registry.assign<comp::Pipeline>(entity, pipeline);
    m_ctx.registry.assign<comp::Position>(entity, worldPos);
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::update() {
    m_ctx.registry.view<comp::VertexBuffer, comp::Pipeline, comp::Position>()
    .each([&](met::entity entity, comp::VertexBuffer& vb, comp::Pipeline& pipeline, comp::Position& position) {
        m_ctx.rcommand.bindVertexBuffer(vb);
        m_ctx.rcommand.bindPipeline(pipeline);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    });
}
