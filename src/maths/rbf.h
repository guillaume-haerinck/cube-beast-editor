#pragma once

#include <Eigen/Dense>

namespace voxmt {
    unsigned int linear(int x);
    unsigned int multiquadratic(int x, float eps);
    unsigned int inverseQuadratic(int x, float eps);
    unsigned int inverseMultiquadratic(int x, float eps);
    unsigned int gaussian(int x, float eps); 

    // VectorXd VectorWi(const int weight, const Point3D listepoints, const float eps);
    // VectorXd interpolation(const VectorXd &values, const Point3D &listepoints, const std::string &RBF, const float eps, const VectorXd W);
}
