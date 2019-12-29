#include "brush-gui.h"

#include <imgui/imgui.h>
#include <imgui_internal.h>

#include "gui/icons-awesome.h"

BrushGui::BrushGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

BrushGui::~BrushGui() {}

void BrushGui::update() {
    ImGui::Begin(ICON_FA_BRUSH "  Brush", 0);
    {
        ImGui::Text("  Usage");

        
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, m_scomps.uiStyle.largeButtonRounding());
        {
            if (drawButton(ICON_FA_MOUSE_POINTER, "Select", m_scomps.brush.usage() == BrushUse::SELECT)) {
                m_scomps.brush.m_usage = BrushUse::SELECT;
            }
            if (drawButton(ICON_FA_PEN, "Add", m_scomps.brush.usage() == BrushUse::ADD)) {
                m_scomps.brush.m_usage = BrushUse::ADD;
            }
            if (drawButton(ICON_FA_PAINT_BRUSH, "Paint", m_scomps.brush.usage() == BrushUse::PAINT)) {
                m_scomps.brush.m_usage = BrushUse::PAINT;
            }
            if (drawButton(ICON_FA_ERASER, "Erase", m_scomps.brush.usage() == BrushUse::REMOVE)) {
                m_scomps.brush.m_usage = BrushUse::REMOVE;
            }

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Text("  Mode");
            if (drawButton(ICON_FA_CUBE, "Voxel", m_scomps.brush.type() == BrushType::VOXEL)) {
                m_scomps.brush.m_type = BrushType::VOXEL;
            }
            if (drawButton(ICON_FA_VECTOR_SQUARE, "Polygon", m_scomps.brush.type() == BrushType::RECTANGLE)) {
                m_scomps.brush.m_type = BrushType::RECTANGLE;
            }
            if (drawButton(ICON_FA_TH, "Face", m_scomps.brush.type() == BrushType::FACE)) {
                m_scomps.brush.m_type = BrushType::FACE;
            }
            if (drawButton(ICON_FA_SLASH, "Line", m_scomps.brush.type() == BrushType::LINE)) {
                m_scomps.brush.m_type = BrushType::LINE;
            }
            if (drawButton(ICON_FA_STOP_CIRCLE, "Sphere", m_scomps.brush.type() == BrushType::CIRCLE)) {
                m_scomps.brush.m_type = BrushType::CIRCLE;
            }
        }
        ImGui::PopStyleVar(1);

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Options");   
    }
    ImGui::End();
}

void BrushGui::onEvent(GuiEvent e) {

}

bool BrushGui::drawButton(const char* text, const char* tooltip, bool toggled) {
    bool isPressed = false;

    ImGui::PushFont(m_scomps.uiStyle.fontIconLarge());
    if (toggled) {
        ImGui::PushStyleColor(ImGuiCol_Button,  m_scomps.uiStyle.selectColor());
        if (ImGui::Button(text, m_scomps.uiStyle.largeButtonSize())) {
            isPressed = true;
        }
        ImGui::PopStyleColor(1);
    } else {
        if (ImGui::Button(text, m_scomps.uiStyle.largeButtonSize())) {
            isPressed = true;
        }
    }
    ImGui::PopFont();

    if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.3f) {
        ImGui::SetTooltip(tooltip);
    }

    return isPressed;
}
