#include "palette-gui.h"

#include <imgui/imgui.h>

#include "gui/icons-awesome.h"

PaletteGui::PaletteGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) 
{
    m_backupColor = m_scomps.materials.m_materials.at(m_scomps.materials.selectedIndex()).albedo;
}

PaletteGui::~PaletteGui() {}

void PaletteGui::update() {
    ImGui::Begin(ICON_FA_PALETTE "  Palette", 0);
    {
        ImGui::BeginGroup();
        {
            auto selected = m_scomps.materials.getSelected().albedo;
            ImVec4 color = ImVec4(selected.r, selected.g, selected.b, 1.0f);
            ImVec4 backup = ImVec4(m_backupColor.r, m_backupColor.g, m_backupColor.b, 1.0f);
            ImGui::ColorButton("##current", color, ImGuiColorEditFlags_NoPicker, ImVec2(60, 40));
            if (ImGui::ColorButton("##previous", backup, ImGuiColorEditFlags_NoPicker, ImVec2(20, 20))) {
                m_scomps.materials.m_materials.at(m_scomps.materials.selectedIndex()).albedo = m_backupColor;
                m_backupColor = selected;
                m_scomps.materials.m_hasToBeUpdated = true;
            }
        }
        ImGui::EndGroup();

        // TODO color picker with eye drop & emissive factor & naming ?

        ImGui::SameLine();
        ImGui::ColorPicker3(
            "##picker", 
            &m_scomps.materials.m_materials.at(m_scomps.materials.m_selectedIndex).albedo.r,
            ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview
        );
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            m_scomps.materials.m_hasToBeUpdated = true;
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        ImGuiStyle& style = ImGui::GetStyle();
        float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        for (unsigned int i = 0; i < m_scomps.materials.size(); i++) {
            const auto& albedo = m_scomps.materials.at(i).albedo;
            const ImVec4 color = ImVec4(albedo.r, albedo.g, albedo.b, 1.0f);

            ImGui::PushID(i);
            if (i == m_scomps.materials.selectedIndex()) {
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3.0f);
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 1));
                    ImGui::ColorButton("", color, ImGuiColorEditFlags_NoAlpha, m_scomps.uiStyle.mediumButtonSize());
                ImGui::PopStyleColor(1);
                ImGui::PopStyleVar(1);
            } else {
                if (ImGui::ColorButton("", color, ImGuiColorEditFlags_NoAlpha, m_scomps.uiStyle.mediumButtonSize())) {
                    m_scomps.materials.m_selectedIndex = i;
                    m_backupColor = m_scomps.materials.m_materials.at(m_scomps.materials.selectedIndex()).albedo;
                }
            }
            
            float last_button_x2 = ImGui::GetItemRectMax().x;
            float next_button_x2 = last_button_x2 + style.ItemSpacing.x + m_scomps.uiStyle.mediumButtonSize().x;
            if (i + 1 < m_scomps.materials.size() && next_button_x2 < window_visible_x2)
                ImGui::SameLine();

            ImGui::PopID();
        }
    }
    ImGui::End();
}

void PaletteGui::onEvent(GuiEvent e) {

}
