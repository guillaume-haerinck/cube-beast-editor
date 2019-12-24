#include "tools-gui.h"

ToolsGui::ToolsGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

ToolsGui::~ToolsGui() {}

void ToolsGui::update() {
    ImGui::Begin("Tools", 0);
        ImGui::Text("Hello world");
    ImGui::End();
}

void ToolsGui::onEvent(GuiEvent e) {
    switch (e) {
        case GuiEvent::APP_LAUNCHED: break;
    }
}
