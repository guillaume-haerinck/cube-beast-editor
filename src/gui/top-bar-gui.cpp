#include "top-bar-gui.h"

#include <imgui/imgui_internal.h>

TopBarGui::TopBarGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps), m_setDefaultLayout(true) {}

TopBarGui::~TopBarGui() {}

void TopBarGui::update() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("TopBar", 0, window_flags);
    {
        ImGui::PopStyleVar(3);

        m_dockspaceId = ImGui::GetID("DockSpace");
        ImGui::DockSpace(m_dockspaceId, ImVec2(0.0f, 0.0f));

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::MenuItem("Yes it works");
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }
    ImGui::End();

    if (m_setDefaultLayout) {
        m_setDefaultLayout = false;
        setDefaultLayout();
    }
}

void TopBarGui::onEvent(GuiEvent e) {
    switch (e) {
        case GuiEvent::APP_LAUNCHED: break;
    }
}

void TopBarGui::setDefaultLayout() {
    ImGui::DockBuilderRemoveNode(m_dockspaceId); // Clear out existing layout
    ImGui::DockBuilderAddNode(m_dockspaceId, ImGuiDockNodeFlags_DockSpace); // Add empty node
    ImGui::DockBuilderSetNodeSize(m_dockspaceId, ImVec2(500, 500));

    // Available positions
    ImGuiID dock_main_id = m_dockspaceId;
    ImGuiID dock_top_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.5f, nullptr, &dock_main_id);
	ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
	ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.5f, nullptr, &dock_main_id);
	ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.5f, nullptr, &dock_main_id);
    ImGuiID dock_center_id = ImGui::DockBuilderGetCentralNode(m_dockspaceId)->ID;

    // Place GUIs
    ImGui::DockBuilderDockWindow("Viewport", dock_top_id);
    ImGui::DockBuilderDockWindow("Main debug window", dock_center_id);
    ImGui::DockBuilderDockWindow("Dear ImGui Demo", dock_right_id);

    // Set appearance
    ImGuiDockNode* node = ImGui::DockBuilderGetNode(dock_top_id);
	node->LocalFlags |= ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoDockingInCentralNode;
    
    ImGui::DockBuilderFinish(m_dockspaceId);
}
