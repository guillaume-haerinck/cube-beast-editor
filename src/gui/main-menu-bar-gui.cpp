#include "main-menu-bar-gui.h"

#include <imgui/imgui_internal.h>

#ifndef __EMSCRIPTEN__
    #include <tinyfiledialogs/tinyfiledialogs.h>
#endif

#include "icons-awesome.h"
#include "loaders/cbe-loader.h"


MainMenuBarGui::MainMenuBarGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps), m_setDefaultLayout(true) {}

MainMenuBarGui::~MainMenuBarGui() {}

void MainMenuBarGui::update() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Main Menu Bar", 0, window_flags);
    {
        ImGui::PopStyleVar(3);

        m_dockspaceId = ImGui::GetID("DockSpace");
        ImGui::DockSpace(m_dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {

#ifndef __EMSCRIPTEN__
                if (ImGui::MenuItem("Open model")) {
                    char const* filters[1] = { "*.cbe" };
                    const char* filePath = tinyfd_openFileDialog("Load a .cbe model", "", 1, filters, 0, 0);
                    if (filePath != nullptr) {
                        CbeLoader loader(m_ctx);
                        loader.loadFile(filePath);
                    }
                }
#endif
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
    ImGui::End();

    if (m_setDefaultLayout) {
        m_setDefaultLayout = false;
        setDefaultLayout();
        setDefaultStyle();
    }
}

void MainMenuBarGui::onEvent(GuiEvent e) {

}

void MainMenuBarGui::setDefaultLayout() {
    ImGui::DockBuilderRemoveNode(m_dockspaceId); // Clear out existing layout
    ImGui::DockBuilderAddNode(m_dockspaceId, ImGuiDockNodeFlags_DockSpace); // Add empty node
    ImGui::DockBuilderSetNodeSize(m_dockspaceId, ImVec2(500, 500));

    // Available positions
    ImGuiID dock_main_id = m_dockspaceId;
    ImGuiID dock_full_up_id;
	ImGuiID dock_full_down_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.05f, nullptr, &dock_full_up_id);
    
    ImGuiID dock_half_right_id;
    ImGuiID dock_half_left_id = ImGui::DockBuilderSplitNode(dock_full_up_id, ImGuiDir_Left, 0.9f, nullptr, &dock_half_right_id);
    ImGuiID dock_half_left_down_id;
    ImGuiID dock_half_left_up_id = ImGui::DockBuilderSplitNode(dock_half_left_id, ImGuiDir_Up, 0.05f, nullptr, &dock_half_left_down_id);
    ImGuiID dock_half_left_left_id;
    ImGuiID dock_half_left_right_id = ImGui::DockBuilderSplitNode(dock_half_left_down_id, ImGuiDir_Right, 0.9f, nullptr, &dock_half_left_left_id);

    ImGuiID dock_half_right_down_id;
    ImGuiID dock_half_right_up_id = ImGui::DockBuilderSplitNode(dock_half_right_id, ImGuiDir_Up, 0.4f, nullptr, &dock_half_right_down_id);

    // Place GUIs
    ImGui::DockBuilderDockWindow("Context Info Bar", dock_full_down_id);
    ImGui::DockBuilderDockWindow("Viewport Option Bar", dock_half_left_up_id);

    ImGui::DockBuilderDockWindow("Viewport", dock_half_left_right_id);
    ImGui::DockBuilderDockWindow(ICON_FA_BRUSH "  Brush", dock_half_left_left_id);

    ImGui::DockBuilderDockWindow(ICON_FA_GLOBE_AMERICAS "  Scene Outline", dock_half_right_up_id);
    ImGui::DockBuilderDockWindow(ICON_FA_PALETTE "  Palette", dock_half_right_down_id);
    ImGui::DockBuilderDockWindow(ICON_FA_SEEDLING "  Generation", dock_half_right_down_id);
    
    // Set appearance
    ImGui::DockBuilderGetNode(dock_main_id)->LocalFlags |= ImGuiDockNodeFlags_NoSplit;
    ImGui::DockBuilderGetNode(dock_half_left_up_id)->LocalFlags |= ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking | ImGuiDockNodeFlags_NoSplit;
    ImGui::DockBuilderGetNode(dock_full_down_id)->LocalFlags |= ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking | ImGuiDockNodeFlags_NoSplit;
	ImGui::DockBuilderGetNode(dock_half_left_right_id)->LocalFlags |= ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking;
    ImGui::DockBuilderGetNode(dock_half_left_left_id)->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;

    ImGui::DockBuilderFinish(m_dockspaceId);
}

void MainMenuBarGui::setDefaultStyle() {
    ImGui::GetStyle().FrameRounding = 3.0f;
    ImGui::GetStyle().GrabRounding = 3.0f;
    ImGui::GetStyle().TabRounding = 3.0f;
    ImGui::GetStyle().WindowRounding = 3.0f;
    ImGui::GetStyle().ScrollbarRounding = 3.0f;

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}
