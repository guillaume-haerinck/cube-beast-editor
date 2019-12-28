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
        }
        if (ImGui::Button("Add")) {
            m_scomps.brush.m_usage = BrushUse::ADD;
        }
        if (ImGui::ImageButton((void*) m_scomps.textures.at(TextureIndex::UI).id, ImVec2(25, 25))) {
            m_scomps.brush.m_usage = BrushUse::REMOVE;
        }
        if (ImGui::Button("Paint")) {
            m_scomps.brush.m_usage = BrushUse::PAINT;
        }

        ImGui::Separator();

        ImGui::Text("Mode");
        if (ImGui::Button("Voxel")) {
            m_scomps.brush.m_type = BrushType::VOXEL;
        }
        if (ImGui::Button("Rectangle")) {
            m_scomps.brush.m_type = BrushType::RECTANGLE;
        }
        if (ImGui::Button("Face")) {
            m_scomps.brush.m_type = BrushType::FACE;
        }
        if (ImGui::Button("Line")) {
            m_scomps.brush.m_type = BrushType::LINE;
        }
        if (ImGui::Button("Circle")) {
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
