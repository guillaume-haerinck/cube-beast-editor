#include "brush-gui.h"

#include <imgui/imgui.h>

BrushGui::BrushGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

BrushGui::~BrushGui() {}

void BrushGui::update() {
    ImGui::Begin("Brush", 0);
        ImGui::Text("Usage");
        if (ImGui::ImageButton((void*) m_scomps.textures.at(TextureIndex::UI).id, ImVec2(48, 48), ImVec2(0.8f, 0.2f), ImVec2(1.0f, 0.4f))) {
            m_scomps.brush.m_usage = BrushUse::SELECT;
        }
        if (ImGui::ImageButton((void*) m_scomps.textures.at(TextureIndex::UI).id, ImVec2(48, 48), ImVec2(0, 0), ImVec2(0.2f, 0.2f))) {
            m_scomps.brush.m_usage = BrushUse::ADD;
        }
        if (ImGui::ImageButton((void*) m_scomps.textures.at(TextureIndex::UI).id, ImVec2(48, 48), ImVec2(0.6f, 0), ImVec2(0.8f, 0.2f))) {
            m_scomps.brush.m_usage = BrushUse::REMOVE;
        }
        if (ImGui::ImageButton((void*) m_scomps.textures.at(TextureIndex::UI).id, ImVec2(48, 48), ImVec2(0.4f, 0.6f), ImVec2(0.6f, 0.8f))) {
            m_scomps.brush.m_usage = BrushUse::PAINT;
        }

        ImGui::Separator();

        ImGui::Text("Mode");
        if (ImGui::ImageButton((void*) m_scomps.textures.at(TextureIndex::UI).id, ImVec2(48, 48), ImVec2(0.4f, 0.8f), ImVec2(0.6f, 1.0f))) {
            m_scomps.brush.m_type = BrushType::VOXEL;
        }
        if (ImGui::ImageButton((void*) m_scomps.textures.at(TextureIndex::UI).id, ImVec2(48, 48), ImVec2(0.8f, 0), ImVec2(1.0f, 0.2f))) {
            m_scomps.brush.m_type = BrushType::RECTANGLE;
        }
        if (ImGui::ImageButton((void*) m_scomps.textures.at(TextureIndex::UI).id, ImVec2(48, 48), ImVec2(0, 0.2f), ImVec2(0.2f, 0.4f))) {
            m_scomps.brush.m_type = BrushType::FACE;
        }
        if (ImGui::ImageButton((void*) m_scomps.textures.at(TextureIndex::UI).id, ImVec2(48, 48), ImVec2(0, 0.4f), ImVec2(0.2f, 0.6f))) {
            m_scomps.brush.m_type = BrushType::LINE;
        }
        if (ImGui::ImageButton((void*) m_scomps.textures.at(TextureIndex::UI).id, ImVec2(48, 48), ImVec2(0.2f, 0), ImVec2(0.4f, 0.2f))) {
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
