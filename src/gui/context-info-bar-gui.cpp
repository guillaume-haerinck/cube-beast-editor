#include "context-info-bar-gui.h"

#include <imgui/imgui.h>

ContextInfoBarGui::ContextInfoBarGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

ContextInfoBarGui::~ContextInfoBarGui() {}

void ContextInfoBarGui::update() {
    ImGui::Begin("Context Info Bar", 0);
        ImGui::Text("context info bar");
    ImGui::End();
}

void ContextInfoBarGui::onEvent(GuiEvent e) {

}
