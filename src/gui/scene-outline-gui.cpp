#include "scene-outline-gui.h"

#include <imgui/imgui.h>

SceneOutlineGui::SceneOutlineGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

SceneOutlineGui::~SceneOutlineGui() {}

void SceneOutlineGui::update() {
    ImGui::Begin("Scene Outline", 0);
        ImGui::Text("chooose an object");
    ImGui::End();
}

void SceneOutlineGui::onEvent(GuiEvent e) {

}
