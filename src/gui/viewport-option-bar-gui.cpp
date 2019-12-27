#include "viewport-option-bar-gui.h"

#include <imgui/imgui.h>

ViewportOptionBarGui::ViewportOptionBarGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

ViewportOptionBarGui::~ViewportOptionBarGui() {}

void ViewportOptionBarGui::update() {
    ImGui::Begin("Viewport Option Bar", 0);
        ImGui::Text("set options");
    ImGui::End();
}

void ViewportOptionBarGui::onEvent(GuiEvent e) {
    switch (e) {
        case GuiEvent::APP_LAUNCHED: break;
    }
}
