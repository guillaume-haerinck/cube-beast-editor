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
			m_scomps.viewport.m_isHovered = true;
		} else {
			m_scomps.viewport.m_isHovered = false;
		}

        // Handle framebuffer
        ImVec2 viewportSize = ImGui::GetWindowSize();
        if (viewportSize.x != m_scomps.viewport.size().x || viewportSize.y != m_scomps.viewport.size().y) {
            m_scomps.viewport.m_size = glm::ivec2(viewportSize.x, viewportSize.y);
            glViewport(0, 0, viewportSize.x, viewportSize.y);
            m_scomps.camera.m_proj = glm::perspectiveFovLH(glm::quarter_pi<float>(), (float) viewportSize.x, (float) viewportSize.y, 0.1f, 100.0f);
            
			// Remake Framebuffers
            // TODO do it only after a delay
            m_scomps.renderTargets.destroy(m_ctx.rcommand);
            m_scomps.renderTargets.init(m_ctx.rcommand, m_scomps.viewport);
        }

        // Update viewport position & draw framebuffer
        {
            const ImVec2 viewportPosTopLeft = ImGui::GetCursorScreenPos();
            m_scomps.viewport.m_posTopLeft = glm::ivec2(viewportPosTopLeft.x, viewportPosTopLeft.y);
			ImGui::GetWindowDrawList()->AddImage(
				(void*) m_scomps.renderTargets.at(RenderTargetIndex::RTT_FINAL).textureIds.at(0),
				viewportPosTopLeft,
				ImVec2(viewportPosTopLeft.x + viewportSize.x, viewportPosTopLeft.y + viewportSize.y),
				ImVec2(0, 1), ImVec2(1, 0)
			);
        }
    }
    ImGui::End();
}

void ViewportGui::onEvent(GuiEvent e) {
    switch(e) {
        default: break;
    }
}
