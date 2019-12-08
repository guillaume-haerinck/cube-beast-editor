#include "viewport-layer.h"

#include <met/met.hpp>
#include <glm/glm.hpp>
#include <imgui.h>

#include "components/graphics/material.h"
#include "components/physics/transform.h"

ViewportLayer::ViewportLayer(Context& ctx) : m_ctx(ctx) {
    comp::Material material;

    for (int i = -1; i < 3; i++) {
        met::entity entity = m_ctx.registry.create();
        m_ctx.registry.assign<comp::Material>(entity, material);
        m_ctx.registry.assign<comp::Transform>(entity, glm::ivec3(i));
    }
}

ViewportLayer::~ViewportLayer() {}

void ViewportLayer::update() {
    ImGui::Begin("Main debug window");
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}
