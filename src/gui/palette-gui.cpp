#include "palette-gui.h"

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gui/icons-awesome.h"

PaletteGui::PaletteGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

PaletteGui::~PaletteGui() {}

void PaletteGui::update() {
    ImGui::Begin(ICON_FA_PALETTE "  Palette", 0);
        ImGuiColorEditFlags misc_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview;
        ImGui::ColorPicker3(
            "##picker", 
            &m_scomps.materials.m_materials.at(m_scomps.materials.m_selectedIndex).albedo.r,
            misc_flags 
        );
        
        ImGuiStyle& style = ImGui::GetStyle();
        misc_flags = ImGuiColorEditFlags_NoAlpha;
        float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
        ImVec2 button_sz = ImVec2(40, 40);

        for (unsigned int i = 0; i <  m_scomps.materials.size(); i++) {
            const auto& albedo = m_scomps.materials.at(i).albedo;
            const ImVec4 color = ImVec4(albedo.r, albedo.g, albedo.b, 1.0f);

            ImGui::PushID(i);
            if (ImGui::ColorButton("Material" + i, color, misc_flags, button_sz)) {
                m_scomps.materials.m_selectedIndex = i;
            }
            
            float last_button_x2 = ImGui::GetItemRectMax().x;
            float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x;
            if (i + 1 < m_scomps.materials.size() && next_button_x2 < window_visible_x2)
                ImGui::SameLine();

            ImGui::PopID();
        }

        // ImGui::ColorEdit3("MyColor##1", glm::value_ptr(myval));
    ImGui::End();
}

void PaletteGui::onEvent(GuiEvent e) {

}
