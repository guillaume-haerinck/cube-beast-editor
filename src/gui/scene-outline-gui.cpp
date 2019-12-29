#include "scene-outline-gui.h"

#include <imgui/imgui.h>
#include "gui/icons-awesome.h"

SceneOutlineGui::SceneOutlineGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

SceneOutlineGui::~SceneOutlineGui() {}

void SceneOutlineGui::update() {
    ImGui::Begin(ICON_FA_GLOBE_AMERICAS "  Scene Outline", 0);
        ImGui::Text("chooose an object");
    ImGui::End();
}

void SceneOutlineGui::onEvent(GuiEvent e) {

}
