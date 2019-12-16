#pragma once

#include <Eigen/Dense>
#include <glm/glm.hpp>
#include <algorithm>
#include <math.h>
namespace voxmt {

    enum class RBFType { LINEAR = 0, MULTIQUADRATIC, INVERSEQUADRATIC, INVERSEMULTIQUAD, GAUSSIAN };

    unsigned int linear(int x);
    unsigned int multiquadratic(int x, float eps);
    unsigned int inverseQuadratic(int x, float eps);
    unsigned int inverseMultiquadratic(int x, float eps);
    unsigned int gaussian(int x, float eps);

    Eigen::VectorXd VectorWi(const Eigen::VectorXd& weight, std::vector<glm::ivec3>& listepoints, RBFType RBF, const float eps);
    std::vector<glm::ivec3> interpolation(std::vector<glm::ivec3>& values, std::vector<glm::ivec3>& listepoints, RBFType RBF, const float eps, const Eigen::VectorXd W);

}
  
