#include "viewport-gui.h"

#include <met/met.hpp>
#include <glm/glm.hpp>
#include <imgui.h>

#include "components/graphics/material.h"
#include "components/physics/transform.h"

ViewportGui::ViewportGui(Context& ctx, SingletonComponents& scomps) : m_ctx(ctx), m_scomps(scomps) {
    comp::Material material;

    met::entity entity = m_ctx.registry.create();
    m_ctx.registry.assign<comp::Material>(entity, material);
    m_ctx.registry.assign<comp::Transform>(entity, glm::ivec3(1, 0, 0));
}

ViewportGui::~ViewportGui() {}

void ViewportGui::update() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    window_flags |= ImGuiWindowFlags_NoBackground;
    
    bool open = true;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace Demo", &open, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

        if(ImGui::BeginMenuBar()){
            if (ImGui::BeginMenu("Docking")) {
                ImGui::MenuItem("Flag: NoSplit");
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Begin("Main debug window");
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Begin("Other window");
            ImGui::Text("hello");
        ImGui::End();

        ImGui::Begin("Game rendering");
        {
            ImGui::GetWindowDrawList()->AddImage(
                (void*) m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_GEOMETRY).textureIds.at(0),
                ImVec2(0, m_scomps.windowSize.y),
                ImVec2(m_scomps.windowSize.y, 0)
            );
        }
        ImGui::End();

    ImGui::End();
}
