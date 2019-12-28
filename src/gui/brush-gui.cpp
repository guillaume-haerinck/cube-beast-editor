#include "brush-gui.h"

#include <imgui/imgui.h>

BrushGui::BrushGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

BrushGui::~BrushGui() {}

void BrushGui::update() {
    ImGui::Begin("Brush", 0);
        ImGui::Text("Usage");
        if (ImGui::Button("Select")) {
            m_scomps.brush.m_usage = BrushUse::SELECT;
        } else if (ImGui::Button("Add")) {
            m_scomps.brush.m_usage = BrushUse::ADD;
        } else if (ImGui::Button("Remove")) {
            m_scomps.brush.m_usage = BrushUse::REMOVE;
        } else if (ImGui::Button("Paint")) {
            m_scomps.brush.m_usage = BrushUse::PAINT;
        }

        ImGui::Separator();

        ImGui::Text("Mode");
        if (ImGui::Button("Voxel")) {
            m_scomps.brush.m_type = BrushType::VOXEL;
        } else if (ImGui::Button("Rectangle")) {
            m_scomps.brush.m_type = BrushType::RECTANGLE;
        } else if (ImGui::Button("Face")) {
            m_scomps.brush.m_type = BrushType::FACE;
        } else if (ImGui::Button("Line")) {
            m_scomps.brush.m_type = BrushType::LINE;
        } else if (ImGui::Button("Circle")) {
            m_scomps.brush.m_type = BrushType::CIRCLE;
        }

        /*
        ImGui::Separator();

        ImGui::Text("Options");
        // TODO contextual tool option if available
        */
        
    ImGui::End();
}

void BrushGui::onEvent(GuiEvent e) {

}
