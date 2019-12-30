#pragma once

#include <Eigen/Dense>
#include <glm/glm.hpp>
#include <vector>

namespace voxmt {

    enum class RBFType { LINEAR = 0, MULTIQUADRATIC, INVERSEQUADRATIC, INVERSEMULTIQUAD, GAUSSIAN };

    float linear(float x);
    float multiquadratic(float x, float eps);
    float inverseQuadratic(float x, float eps);
    float inverseMultiquadratic(float x, float eps);
    float gaussian(float x, float eps);
    float distance(const glm::ivec3& point1, const glm::ivec3& point2);

    Eigen::VectorXd vectorWi(const Eigen::VectorXd& weight, const std::vector<glm::ivec3>& listepoints, RBFType RBF, const float eps);
    std::vector<glm::ivec3> interpolation(const std::vector<glm::ivec3>& values, const std::vector<glm::ivec3>& listepoints, RBFType RBF, const float eps, const Eigen::VectorXd& W);

}
  
