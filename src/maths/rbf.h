#pragma once

#include <Eigen/Dense>
#include <glm/glm.hpp>
#include <vector>

namespace voxmt {

    enum class RBFType { LINEAR = 0, MULTIQUADRATIC, INVERSEQUADRATIC, INVERSEMULTIQUAD, GAUSSIAN };
    enum class RBFTransformAxis { X = 0, Y, Z };

    /**
     * @brief 
     * 
     * @param coordWithOneAxisToFind - 
     * @param controlPointCoords - 
     * @param controlPointWeights -
     * @param type - (Optionnal)
     * @param epsilon - (Optionnal) Must be between 0.0f and 1.0f
     * @param axis - 
     */
    void rbfInterpolate(std::vector<glm::ivec3>& coordWithOneAxisToFind, const std::vector<glm::ivec3>& controlPointCoords, const Eigen::VectorXd& controlPointWeights, const RBFType type = RBFType::LINEAR, const float epsilon = 0.5f, const RBFTransformAxis axis = RBFTransformAxis::Y);

}
  
