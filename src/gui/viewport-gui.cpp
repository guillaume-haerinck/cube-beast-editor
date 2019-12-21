#include "viewport-gui.h"

#include <met/met.hpp>
#include <glm/glm.hpp>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "components/graphics/material.h"
#include "components/physics/transform.h"


// Temp
#ifdef __EMSCRIPTEN__
	#include <GLES3/gl3.h>
#else
	#include <glad/gles2.h>
#endif

ViewportGui::ViewportGui(Context& ctx, SingletonComponents& scomps) : m_ctx(ctx), m_scomps(scomps) {
    comp::Material material;

    met::entity entity = m_ctx.registry.create();
    m_ctx.registry.assign<comp::Material>(entity, material);
    m_ctx.registry.assign<comp::Transform>(entity, glm::ivec3(1, 0, 0));
}

ViewportGui::~ViewportGui() {}

void ViewportGui::update() {
    // Dockspace
    // TODO put this in app ? Because dockspace must be called first
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        // window_flags |= ImGuiWindowFlags_NoBackground;

        bool open = true;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace Demo", &open, window_flags);
        {
            ImGui::PopStyleVar(3);

            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    ImGui::MenuItem("Yes it works");
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
        }
        ImGui::End();
    }

    // Viewport
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;

        bool open = true;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("Viewport", &open, window_flags);
        {
            // Handle framebuffer
            ImVec2 viewportSize = ImGui::GetWindowSize();
            if (viewportSize.x != m_scomps.viewportSize.x || viewportSize.y != m_scomps.viewportSize.y) {
                m_scomps.viewportSize = glm::ivec2(viewportSize.x, viewportSize.y);
                glViewport(0, 0, m_scomps.viewportSize.x, m_scomps.viewportSize.y);
                m_scomps.camera.proj = glm::perspectiveFovLH(glm::quarter_pi<float>(), (float) m_scomps.viewportSize.x, (float) m_scomps.viewportSize.y, 0.1f, 100.0f);
                // RemakeFramebuffers
                {
                    glDeleteFramebuffers(1, &m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_GEOMETRY).frameBufferId);
                    PipelineOutputDescription outputDescription = {
                        { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_Albedo" },
                        { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_Normal" },
                        { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_WorldPosition" },
                        { RenderTargetUsage::Color, RenderTargetType::RenderBuffer, RenderTargetDataType::UCHAR, RenderTargetChannels::RGBA, "EntityIdToColor" },
                        { RenderTargetUsage::Depth, RenderTargetType::RenderBuffer, RenderTargetDataType::FLOAT, RenderTargetChannels::R, "Depth" }
                    };
                    m_ctx.rcommand.createRenderTargets(scomp::RenderTargetsIndex::RTT_GEOMETRY, outputDescription);

                    glDeleteFramebuffers(1, &m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_FINAL).frameBufferId);
                    outputDescription = {
                        { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Color" },
                        { RenderTargetUsage::Depth, RenderTargetType::RenderBuffer, RenderTargetDataType::FLOAT, RenderTargetChannels::R, "Depth" }
                    };
                    m_ctx.rcommand.createRenderTargets(scomp::RenderTargetsIndex::RTT_FINAL, outputDescription);
                }
            }

            // Handle mouseNDC
            {
                ImVec2 viewportPosTopLeft = ImGui::GetCursorScreenPos();
                spdlog::info("{} {}", viewportPosTopLeft.x, viewportPosTopLeft.y);

            
            }

            ImGui::PopStyleVar(3);
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddImage(
                (void*) m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_FINAL).textureIds.at(0),
                ImVec2(pos.x, pos.y),
                ImVec2(pos.x + m_scomps.viewportSize.x, pos.y + m_scomps.viewportSize.y),
                ImVec2(0, 1), ImVec2(1, 0)
            );
        }
        ImGui::End();
    }
    
    // Test windows
    {
        ImGui::Begin("Main debug window");
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Begin("Other window");
            ImGui::Text("hello");
        ImGui::End();

        ImGui::Begin("outside window");
            ImGui::Text("oh im outside");
        ImGui::End();
    }
    
}
