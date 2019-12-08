#include "viewport-layer.h"

#include <glm/glm.hpp>
#include <imgui.h>
#include "factories/entities/primitive-factory.h"

ViewportLayer::ViewportLayer(Context& ctx) : m_ctx(ctx) {
    PrimitiveFactory factory(ctx);
    factory.createCube(glm::ivec3(0));
    factory.createCube(glm::ivec3(-1));
    factory.createCube(glm::ivec3(1));
}

ViewportLayer::~ViewportLayer() {}

void ViewportLayer::update() {
    ImGui::Begin("Main debug window");
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}
