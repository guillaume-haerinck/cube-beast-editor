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
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Viewport", 0, window_flags);
    {
		ImGui::PopStyleVar(1);

		if (ImGui::IsWindowHovered()) {
			//m_scomps.isViewportHovered = true;
		} else {
			//m_scomps.isViewportHovered = false;
		}

        // Handle framebuffer
        ImVec2 viewportSize = ImGui::GetWindowSize();
		/*
        if (viewportSize.x != m_scomps.viewportSize.x || viewportSize.y != m_scomps.viewportSize.y) {
            m_scomps.viewportSize = glm::ivec2(viewportSize.x, viewportSize.y);
            glViewport(0, 0, m_scomps.viewportSize.x, m_scomps.viewportSize.y);
            m_scomps.camera.m_proj = glm::perspectiveFovLH(glm::quarter_pi<float>(), (float) m_scomps.viewportSize.x, (float) m_scomps.viewportSize.y, 0.1f, 100.0f);
            // TODO abstract
			// RemakeFramebuffers
            {
                //glDeleteFramebuffers(1, &m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_GEOMETRY).frameBufferId);
                PipelineOutputDescription outputDescription = {
                    { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_Albedo" },
                    { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_Normal" },
                    { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_WorldPosition" },
                    { RenderTargetUsage::Color, RenderTargetType::RenderBuffer, RenderTargetDataType::UCHAR, RenderTargetChannels::RGBA, "EntityIdToColor" },
                    { RenderTargetUsage::Depth, RenderTargetType::RenderBuffer, RenderTargetDataType::FLOAT, RenderTargetChannels::R, "Depth" }
                };
                //m_ctx.rcommand.createRenderTargets(scomp::RenderTargetsIndex::RTT_GEOMETRY, outputDescription);

                //glDeleteFramebuffers(1, &m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_FINAL).frameBufferId);
                outputDescription = {
                    { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Color" },
                    { RenderTargetUsage::Depth, RenderTargetType::RenderBuffer, RenderTargetDataType::FLOAT, RenderTargetChannels::R, "Depth" }
                };
                //m_ctx.rcommand.createRenderTargets(scomp::RenderTargetsIndex::RTT_FINAL, outputDescription);
            }
        }

        // Update viewport position & draw framebuffer
        {
            const ImVec2 viewportPosTopLeft = ImGui::GetCursorScreenPos();
            m_scomps.viewportPosTopLeft = glm::ivec2(viewportPosTopLeft.x, viewportPosTopLeft.y);
			/*
			ImGui::GetWindowDrawList()->AddImage(
				(void*) m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_FINAL).textureIds.at(0),
				viewportPosTopLeft,
				ImVec2(viewportPosTopLeft.x + m_scomps.viewportSize.x, viewportPosTopLeft.y + m_scomps.viewportSize.y),
				ImVec2(0, 1), ImVec2(1, 0)
			);
        }
		*/
    }
    ImGui::End();
}

void ViewportGui::onEvent(GuiEvent e) {
    switch(e) {
        default: break;
    }
}
