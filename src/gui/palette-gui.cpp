#include "palette-gui.h"

#include <imgui/imgui.h>

PaletteGui::PaletteGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

PaletteGui::~PaletteGui() {}

void PaletteGui::update() {
    ImGui::Begin("Palette", 0);
        ImGui::Text("pick a color");
    ImGui::End();
}

void PaletteGui::onEvent(GuiEvent e) {

}
