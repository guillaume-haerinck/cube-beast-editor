#include "generation-gui.h"

#include <imgui/imgui.h>
#include <Eigen/Dense>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "gui/icons-awesome.h"
#include "maths/rbf.h"

GenerationGui::GenerationGui(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps)
{
    m_controlPointsXYZ.push_back(glm::ivec3(10, 10, 10));
    m_controlPointsWeights.push_back(5);
    m_controlPointsXYZ.push_back(glm::ivec3(0, 0, 0));
    m_controlPointsWeights.push_back(5);
}

GenerationGui::~GenerationGui() {}

void GenerationGui::update() {
    ImGui::Begin(ICON_FA_SEEDLING "  Generation", 0);
        if (ImGui::Button("Add control point")) {
            m_controlPointsXYZ.push_back(glm::ivec3(0, 0, 0));
            m_controlPointsWeights.push_back(1);
        }

        if (m_controlPointsXYZ.size() > 2)
            if (ImGui::Button("Remove control point")) {
                m_controlPointsXYZ.pop_back();
                m_controlPointsWeights.push_back(1);
            }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        for (unsigned int i = 0; i < m_controlPointsXYZ.size(); i++) {
            ImGui::SliderFloat(std::to_string(i).c_str(), &m_controlPointsWeights.at(i), 1, 30);
            ImGui::SliderInt3(std::to_string(i).c_str(), (int*) glm::value_ptr(m_controlPointsXYZ.at(i)), 0, 20);
            ImGui::Separator();
        }

        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button("Generate")) {
            Eigen::VectorXd controlPointWeights(m_controlPointsXYZ.size());
            for (unsigned int i = 0; i < m_controlPointsWeights.size(); i++) {
                controlPointWeights[i] = m_controlPointsWeights.at(i);
            }

            std::vector<glm::ivec3> coordWithYtoFind;
            // TODO fill with every cubes
            voxmt::rbfInterpolate(coordWithYtoFind, m_controlPointsXYZ, controlPointWeights, voxmt::RBFType::LINEAR, 0.5f, voxmt::RBFTransformAxis::Y);

            for (size_t i = 0; i < coordWithYtoFind.size(); i++) {
                
                // trans.position = coordWithYtoFind.at(i);
            }
            
        }
    ImGui::End();
}

void GenerationGui::onEvent(GuiEvent e) {

}
