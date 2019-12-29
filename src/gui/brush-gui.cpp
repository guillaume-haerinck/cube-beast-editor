#include "brush-gui.h"

#include <imgui/imgui.h>

#include "gui/icons-awesome.h"

BrushGui::BrushGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

BrushGui::~BrushGui() {}

void BrushGui::update() {
    ImGui::Begin(ICON_FA_BRUSH "  Brush", 0);
    {
        ImGui::Text("  Usage");
        ImGui::PushFont(m_scomps.uiStyle.fontIconLarge());
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, m_scomps.uiStyle.largeButtonRounding());
        {
            if (drawButton(ICON_FA_MOUSE_POINTER, m_scomps.brush.usage() == BrushUse::SELECT)) {
                m_scomps.brush.m_usage = BrushUse::SELECT;
            }
            if (drawButton(ICON_FA_PEN, m_scomps.brush.usage() == BrushUse::ADD)) {
                m_scomps.brush.m_usage = BrushUse::ADD;
            }
            if (drawButton(ICON_FA_PAINT_BRUSH, m_scomps.brush.usage() == BrushUse::PAINT)) {
                m_scomps.brush.m_usage = BrushUse::PAINT;
            }
            if (drawButton(ICON_FA_ERASER, m_scomps.brush.usage() == BrushUse::REMOVE)) {
                m_scomps.brush.m_usage = BrushUse::REMOVE;
            }

            ImGui::PopFont();

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Text("  Mode");
            ImGui::PushFont(m_scomps.uiStyle.fontIconLarge());
            if (drawButton(ICON_FA_CUBE, m_scomps.brush.type() == BrushType::VOXEL)) {
                m_scomps.brush.m_type = BrushType::VOXEL;
            }
            if (drawButton(ICON_FA_VECTOR_SQUARE, m_scomps.brush.type() == BrushType::RECTANGLE)) {
                m_scomps.brush.m_type = BrushType::RECTANGLE;
            }
            if (drawButton(ICON_FA_TH, m_scomps.brush.type() == BrushType::FACE)) {
                m_scomps.brush.m_type = BrushType::FACE;
            }
            if (drawButton(ICON_FA_SLASH, m_scomps.brush.type() == BrushType::LINE)) {
                m_scomps.brush.m_type = BrushType::LINE;
            }
            if (drawButton(ICON_FA_STOP_CIRCLE, m_scomps.brush.type() == BrushType::CIRCLE)) {
                m_scomps.brush.m_type = BrushType::CIRCLE;
            }
            ImGui::PopFont();
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

bool BrushGui::drawButton(const char* text, bool toggled) {
    bool isPressed = false;

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

    return isPressed;
}
