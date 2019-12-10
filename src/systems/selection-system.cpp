#include "selection-system.h"

// Temp
#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <glad/gles2.h>
#endif

#include <spdlog/spdlog.h>
#include "graphics/gl-exception.h"

SelectionSystem::SelectionSystem(Context& ctx, SingletonComponents& scomps) : m_ctx(ctx), m_scomps(scomps) {}

SelectionSystem::~SelectionSystem() {}

void SelectionSystem::update() {
    // TODO cleanup
    m_ctx.rcommand.bindRenderTargets(m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_GEOMETRY));

    int pixel = 0;
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    GLCall(glReadPixels(m_scomps.inputs.mousePos.x, 500 - m_scomps.inputs.mousePos.y, 1, 1, GL_RED, GL_UNSIGNED_BYTE, &pixel));
    if (pixel != lastSelect) {
        spdlog::info("Entity selected is: {}", pixel);
        lastSelect = pixel;
    }
}
