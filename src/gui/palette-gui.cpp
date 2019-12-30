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
        
        misc_flags = ImGuiColorEditFlags_NoAlpha;
        unsigned int i = 0;
        for (auto& material : m_scomps.materials) {
            const ImVec4 color = ImVec4(material.albedo.r, material.albedo.g, material.albedo.b, 1.0f);
            if (ImGui::ColorButton("Material" + i, color, misc_flags, ImVec2(40, 40))) {
                m_scomps.materials.m_selectedIndex = i;
            }
            i++;
        }

        // ImGui::ColorEdit3("MyColor##1", glm::value_ptr(myval));
    ImGui::End();
}

void PaletteGui::onEvent(GuiEvent e) {

}
