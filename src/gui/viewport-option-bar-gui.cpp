#include "viewport-option-bar-gui.h"

#include <imgui/imgui.h>
#include <imgui_internal.h>

#include "gui/icons-awesome.h"

ViewportOptionBarGui::ViewportOptionBarGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

ViewportOptionBarGui::~ViewportOptionBarGui() {}

void ViewportOptionBarGui::update() {
    ImGui::Begin("Viewport Option Bar", 0);
    {
        // TODO outline cube
        // TODO show or hide grid, and ui on the 3d scene

        ImGui::SetNextItemWidth(135.0f);
        const char* items[] = { ICON_FA_EDIT "  Edit Mode", ICON_FA_BOX_OPEN "  Object Mode" };
        static int item_current = 0;
        ImGui::Combo("", &item_current, items, IM_ARRAYSIZE(items));

        ImGui::SameLine();
        if (drawButton(ICON_FA_UNDO, "Undo")) {
            // TODO
        }
        ImGui::SameLine();
        if (drawButton(ICON_FA_REDO, "Redo")) {
            // TODO
        }
    }
    ImGui::End();
}

void ViewportOptionBarGui::onEvent(GuiEvent e) {

}

bool ViewportOptionBarGui::drawButton(const char* text, const char* tooltip) {
    bool isPressed = false;

    if (ImGui::Button(text)) {
        isPressed = true;
    }

    if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.3f) {
        ImGui::SetTooltip(tooltip);
    }

    return isPressed;
}

