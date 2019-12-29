#include "palette-gui.h"

#include <imgui/imgui.h>
#include "gui/icons-awesome.h"

PaletteGui::PaletteGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

PaletteGui::~PaletteGui() {}

void PaletteGui::update() {
    ImGui::Begin(ICON_FA_PALETTE "  Palette", 0);
        ImGui::Text("pick a color");
    ImGui::End();
}

void PaletteGui::onEvent(GuiEvent e) {

}
