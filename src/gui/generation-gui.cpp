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
                glm::ivec3(5, 5, 5),
                glm::ivec3(0, 0, 0)
            };
            Eigen::VectorXd controlPointWeights(controlPointsXYZ.size());
            controlPointWeights << 1.0f, 10.0f;

            std::vector<glm::ivec3> coordWithYtoFind;
            std::vector<met::entity> entityToChange;
            m_ctx.registry.view<comp::Transform>().each([&](met::entity id, comp::Transform& transform){
                coordWithYtoFind.push_back(transform.position);
                entityToChange.push_back(id);
            });
            voxmt::rbfInterpolate(coordWithYtoFind, controlPointsXYZ, controlPointWeights, voxmt::RBFType::LINEAR, 0.5f, voxmt::RBFTransformAxis::Y);

            for (size_t i = 0; i < coordWithYtoFind.size(); i++) {
                comp::Transform& trans =  m_ctx.registry.get<comp::Transform>(entityToChange.at(i));
                trans.position = coordWithYtoFind.at(i);
            }
            
        }
    ImGui::End();
}

void GenerationGui::onEvent(GuiEvent e) {

}
