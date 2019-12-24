#include "tools-gui.h"

ToolsGui::ToolsGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

ToolsGui::~ToolsGui() {}

void ToolsGui::update() {
    // TODO use ImGui::ImageButton

    ImGui::Begin("Tools", 0);
        if (ImGui::Button("Pencil")) {
            m_scomps.currentBrush = BrushType::VOXEL;
            m_scomps.currentBushUse = BrushUse::ADD;
        } else if (ImGui::Button("Eraser")) {
            m_scomps.currentBrush = BrushType::VOXEL;
            m_scomps.currentBushUse = BrushUse::REMOVE;
        }
    ImGui::End();
}

void ToolsGui::onEvent(GuiEvent e) {
    switch (e) {
        case GuiEvent::APP_LAUNCHED: break;
    }
}
