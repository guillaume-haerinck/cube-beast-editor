#pragma once

#include <Eigen/Dense>
#include <glm/glm.hpp>
#include <vector>

namespace voxmt {

    enum class RBFType { LINEAR = 0, MULTIQUADRATIC, INVERSEQUADRATIC, INVERSEMULTIQUAD, GAUSSIAN };

    /**
     * @brief 
     * 
     * @param coordXYwheretoGetZ - 
     * @param controlPointCoords - 
     * @param controlPointWeights - (Optionnal)
     * @param type - (Optionnal)
     * @param epsilon - (Optionnal) Must be between 0.0f and 1.0f
     */
    void rbfInterpolate(std::vector<glm::ivec3>& coordXYwheretoGetZ, const std::vector<glm::ivec3>& controlPointCoords, const Eigen::VectorXd& controlPointWeights, RBFType type, const float epsilon);

}
  
