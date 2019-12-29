#include "generation-gui.h"

#include <imgui/imgui.h>
#include "gui/icons-awesome.h"

GenerationGui::GenerationGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

GenerationGui::~GenerationGui() {}

void GenerationGui::update() {
    ImGui::Begin(ICON_FA_SEEDLING "  Generation", 0);
        ImGui::Text("Available soon");
    ImGui::End();
}

void GenerationGui::onEvent(GuiEvent e) {

}
