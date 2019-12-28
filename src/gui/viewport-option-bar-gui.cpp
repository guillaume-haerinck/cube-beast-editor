#include "viewport-option-bar-gui.h"

#include <imgui/imgui.h>

ViewportOptionBarGui::ViewportOptionBarGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

ViewportOptionBarGui::~ViewportOptionBarGui() {}

void ViewportOptionBarGui::update() {
    ImGui::Begin("Viewport Option Bar", 0);
        // TODO switch between edit and object mode
        // TODO outline cube
        // TODO show or hide grid, and ui on the 3d scene

        ImGui::Text("set options");
    ImGui::End();
}

void ViewportOptionBarGui::onEvent(GuiEvent e) {

}
