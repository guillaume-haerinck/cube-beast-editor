#include "selection-system.h"

// Temp
#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <glad/gles2.h>
#endif

#include <spdlog/spdlog.h>
#include "graphics/gl-exception.h"
#include "maths/casting.h"

SelectionSystem::SelectionSystem(Context& ctx, SingletonComponents& scomps) : m_ctx(ctx), m_scomps(scomps), m_lastSelect(0) {}

SelectionSystem::~SelectionSystem() {}

void SelectionSystem::update() {
    // TODO cleanup
    m_ctx.rcommand.bindRenderTargets(m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_GEOMETRY));

    // TODO use blue channel to say which face is selected
    unsigned char pixel[] = { 0, 0, 0 };
    glReadBuffer(GL_COLOR_ATTACHMENT1);
    GLCall(glReadPixels(m_scomps.inputs.mousePos.x, 500 - m_scomps.inputs.mousePos.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel));
    unsigned int selectId = voxmt::colorToInt(pixel[0], pixel[1], 0);
    if (selectId != m_lastSelect) {
        spdlog::info("Entity selected is: {}", selectId);
        m_lastSelect = selectId;
    }

    // TODO raycast on grid if no cube selected
}
