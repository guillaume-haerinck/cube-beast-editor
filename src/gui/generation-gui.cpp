#include "generation-gui.h"

#include <imgui/imgui.h>

GenerationGui::GenerationGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

GenerationGui::~GenerationGui() {}

void GenerationGui::update() {
    ImGui::Begin("Generation", 0);
        ImGui::Text("generate procedural");
    ImGui::End();
}

void GenerationGui::onEvent(GuiEvent e) {

}
