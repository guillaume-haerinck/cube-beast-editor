#include "brush-gui.h"

#include <imgui/imgui.h>

BrushGui::BrushGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

BrushGui::~BrushGui() {}

void BrushGui::update() {
    ImGui::Begin("Brush", 0);
        ImGui::Text("Brushes");
    ImGui::End();
}

void BrushGui::onEvent(GuiEvent e) {
    switch (e) {
        case GuiEvent::APP_LAUNCHED: break;
    }
}
