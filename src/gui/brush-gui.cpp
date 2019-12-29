#include "brush-gui.h"

#include <imgui/imgui.h>

#include "gui/icons-awesome.h"

BrushGui::BrushGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

BrushGui::~BrushGui() {}

void BrushGui::update() {
    ImGui::Begin(ICON_FA_BRUSH "  Brush", 0);
        ImGui::Text("  Usage");
        ImGui::PushFont(m_scomps.uiStyle.fontIconLarge());
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, m_scomps.uiStyle.largeButtonRounding());
        if (ImGui::Button(ICON_FA_MOUSE_POINTER, m_scomps.uiStyle.largeButtonSize())) {
            m_scomps.brush.m_usage = BrushUse::SELECT;
        }
        if (ImGui::Button(ICON_FA_PEN, m_scomps.uiStyle.largeButtonSize())) {
            m_scomps.brush.m_usage = BrushUse::ADD;
        }
        if (ImGui::Button(ICON_FA_ERASER, m_scomps.uiStyle.largeButtonSize())) {
            m_scomps.brush.m_usage = BrushUse::REMOVE;
        }
        if (ImGui::Button(ICON_FA_PAINT_BRUSH, m_scomps.uiStyle.largeButtonSize())) {
            m_scomps.brush.m_usage = BrushUse::PAINT;
        }
        ImGui::PopFont();

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("  Mode");
        ImGui::PushFont(m_scomps.uiStyle.fontIconLarge());
        if (ImGui::Button(ICON_FA_CUBE, m_scomps.uiStyle.largeButtonSize())) {
            m_scomps.brush.m_type = BrushType::VOXEL;
        }
        if (ImGui::Button(ICON_FA_VECTOR_SQUARE, m_scomps.uiStyle.largeButtonSize())) {
            m_scomps.brush.m_type = BrushType::RECTANGLE;
        }
        if (ImGui::Button(ICON_FA_TH, m_scomps.uiStyle.largeButtonSize())) {
            m_scomps.brush.m_type = BrushType::FACE;
        }
        if (ImGui::Button(ICON_FA_SLASH, m_scomps.uiStyle.largeButtonSize())) {
            m_scomps.brush.m_type = BrushType::LINE;
        }
        if (ImGui::Button(ICON_FA_STOP_CIRCLE, m_scomps.uiStyle.largeButtonSize())) {
            m_scomps.brush.m_type = BrushType::CIRCLE;
        }
        ImGui::PopFont();
        ImGui::PopStyleVar(1);

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Options");
        
        
    ImGui::End();
}

void BrushGui::onEvent(GuiEvent e) {

}
