#include "context-info-bar-gui.h"

#include <imgui/imgui.h>
#include "gui/icons-awesome.h"

ContextInfoBarGui::ContextInfoBarGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

ContextInfoBarGui::~ContextInfoBarGui() {}

void ContextInfoBarGui::update() {
    ImGui::Begin("Context Info Bar", 0);
    {
        // Left part
        ImGui::Text(ICON_FA_MOUSE "  "); ImGui::SameLine(0, 0);
        ImGui::Text("L: "); ImGui::SameLine(0, 0);
        switch (m_scomps.brush.usage()) {
            case BrushUse::ADD: ImGui::Text("Add "); break;
            case BrushUse::PAINT: ImGui::Text("Paint "); break;
            case BrushUse::REMOVE: ImGui::Text("Remove "); break;
            case BrushUse::SELECT: ImGui::Text("Select "); break;
            default: ImGui::Text("?"); break;
        }
        ImGui::SameLine(0, 0);

        ImGui::Text("| M: Pan "); ImGui::SameLine(0, 0);
        ImGui::Text("| R: Move "); ImGui::SameLine(0, 0);

        ImGui::Dummy(ImVec2(40.0f, 0.0f)); ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 90.0f, 0);
        
        // Right part
        ImGui::Text("Hovered:");ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 30.0f, 0);
        if (m_scomps.hovered.exist()) {
            ImGui::Text("%d,%d,%d", m_scomps.hovered.position().x, m_scomps.hovered.position().y, m_scomps.hovered.position().z);
        } else {
            ImGui::Text("-,-,-");
        }
    }
    ImGui::End();
}

void ContextInfoBarGui::onEvent(GuiEvent e) {

}
