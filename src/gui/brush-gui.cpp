#include "brush-gui.h"

#include <imgui/imgui.h>

BrushGui::BrushGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

BrushGui::~BrushGui() {}

void BrushGui::update() {
    ImGui::Begin("Brush", 0);
        ImGui::Text("Usage");
        if (ImGui::Button("Select")) {
            m_scomps.currentBushUse = BrushUse::SELECT;
        } else if (ImGui::Button("Add")) {
            m_scomps.currentBushUse = BrushUse::ADD;
        } else if (ImGui::Button("Remove")) {
            m_scomps.currentBushUse = BrushUse::REMOVE;
        } else if (ImGui::Button("Paint")) {
            m_scomps.currentBushUse = BrushUse::PAINT;
        }

        ImGui::Separator();

        ImGui::Text("Mode");
        if (ImGui::Button("Voxel")) {
            m_scomps.currentBrush = BrushType::VOXEL;
        } else if (ImGui::Button("Rectangle")) {
            m_scomps.currentBrush = BrushType::RECTANGLE;
        } else if (ImGui::Button("Face")) {
            m_scomps.currentBrush = BrushType::FACE;
        } else if (ImGui::Button("Line")) {
            m_scomps.currentBrush = BrushType::LINE;
        } else if (ImGui::Button("Circle")) {
            m_scomps.currentBrush = BrushType::CIRCLE;
        }

        /*
        ImGui::Separator();

        ImGui::Text("Options");
        // TODO contextual tool option if available
        */
        
    ImGui::End();
}

void BrushGui::onEvent(GuiEvent e) {
    switch (e) {
        case GuiEvent::APP_LAUNCHED: break;
    }
}
