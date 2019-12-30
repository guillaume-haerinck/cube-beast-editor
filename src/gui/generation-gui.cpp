#include "generation-gui.h"

#include <imgui/imgui.h>
#include <Eigen/Dense>
#include <glm/glm.hpp>

#include "gui/icons-awesome.h"
#include "maths/rbf.h"
#include "components/graphics/material.h"
#include "components/physics/transform.h"

GenerationGui::GenerationGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps) {}

GenerationGui::~GenerationGui() {}

void GenerationGui::update() {
    ImGui::Begin(ICON_FA_SEEDLING "  Generation", 0);
        if (ImGui::Button("Generate")) {
             std::vector<glm::ivec3> controlPointsXYZ = {
                glm::ivec3(1, 1, 3),
                glm::ivec3(0, 0, 8),
                glm::ivec3(2, 2, 2)
            };
            Eigen::VectorXd controlPointWeights(controlPointsXYZ.size());
            controlPointWeights << 1.0f, 1.0f, 1.0f;

            std::vector<glm::ivec3> coordWhereToFindZ;
            std::vector<met::entity> entitiesToChange;
            m_ctx.registry.view<comp::Transform>().each([&](met::entity id, comp::Transform& transform){
                coordWhereToFindZ.push_back(transform.position);
                entitiesToChange.push_back(id);
            });
            voxmt::rbfInterpolate(coordWhereToFindZ, controlPointsXYZ, controlPointWeights, voxmt::RBFType::LINEAR, 0.5f);

            for (size_t i = 0; i < entitiesToChange.size(); i++) {
                comp::Transform& trans = m_ctx.registry.get<comp::Transform>(entitiesToChange.at(i));
                trans.position = coordWhereToFindZ.at(i);
            }
            
        }
    ImGui::End();
}

void GenerationGui::onEvent(GuiEvent e) {

}
