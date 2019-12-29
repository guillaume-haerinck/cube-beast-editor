#include "brush-gui.h"

#include <imgui/imgui.h>

#include "gui/icons-awesome.h"

BrushGui::BrushGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

BrushGui::~BrushGui() {}

void BrushGui::update() {
    ImGui::Begin(ICON_FA_BRUSH " Brush", 0);
        ImGui::Text("Usage");
        if (ImGui::Button(ICON_FA_MOUSE_POINTER)) {
            m_scomps.brush.m_usage = BrushUse::SELECT;
        }
        if (ImGui::Button(ICON_FA_PEN)) {
            m_scomps.brush.m_usage = BrushUse::ADD;
        }
        if (ImGui::Button(ICON_FA_ERASER)) {
            m_scomps.brush.m_usage = BrushUse::REMOVE;
        }
        if (ImGui::Button(ICON_FA_PAINT_BRUSH)) {
            m_scomps.brush.m_usage = BrushUse::PAINT;
        }

        ImGui::Separator();

        ImGui::Text("Mode");
        if (ImGui::Button(ICON_FA_CUBE)) {
            m_scomps.brush.m_type = BrushType::VOXEL;
        }
        if (ImGui::Button(ICON_FA_VECTOR_SQUARE)) {
            m_scomps.brush.m_type = BrushType::RECTANGLE;
        }
        if (ImGui::Button(ICON_FA_TH)) {
            m_scomps.brush.m_type = BrushType::FACE;
        }
        if (ImGui::Button(ICON_FA_SLASH)) {
            m_scomps.brush.m_type = BrushType::LINE;
        }
        if (ImGui::Button(ICON_FA_STOP_CIRCLE)) {
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
