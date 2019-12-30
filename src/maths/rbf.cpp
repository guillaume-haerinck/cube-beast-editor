#include "rbf.h"

#include <algorithm>
#include <math.h>

namespace voxmt {

	unsigned int linear(int x) {
		return x;
	}

	unsigned int multiquadratic(int x, float eps) {
		return (sqrt(1 - (eps * x) * (eps * x)));
	}

	unsigned int inverseQuadratic(int x, float eps) {
		return 1 / (1 - (eps * x) * (eps * x));
	}

	unsigned int inverseMultiquadratic(int x, float eps) {
		return 1 / (sqrt(1 - (eps * x) * (eps * x)));
	}

	unsigned int gaussian(int x, float eps) {
		return exp(-(eps * x) * (eps * x));
	}

	unsigned int distance(const glm::ivec3& point1, const glm::ivec3& point2) {
		return sqrt((point2.x - point1.x) * (point2.x - point1.x) + (point2.y - point1.y) * (point2.y - point1.y) + (point2.z - point1.z) * (point2.z - point1.z));
	}

	Eigen::VectorXd vectorWi(const Eigen::VectorXd& weight, const std::vector<glm::ivec3>& listepoints, RBFType RBF, const float eps) {
		Eigen::MatrixXd D(listepoints.size(), listepoints.size());
		
		for (int i = 0; i < D.rows(); i++) {
			for (int j = 0; j < D.cols(); j++) {
				switch (RBF) {
				case RBFType::LINEAR:
					D(i, j) = voxmt::distance(listepoints.at(i), listepoints.at(j));
					break;
				case RBFType::MULTIQUADRATIC:
					D(i, j) = voxmt::multiquadratic(voxmt::distance(listepoints.at(i), listepoints.at(j)), eps);
					break;
				case RBFType::INVERSEQUADRATIC:
					D(i, j) = voxmt::inverseQuadratic(voxmt::distance(listepoints.at(i), listepoints.at(j)), eps);
					break;
				case RBFType::INVERSEMULTIQUAD:
					D(i, j) = voxmt::inverseMultiquadratic(voxmt::distance(listepoints.at(i), listepoints.at(j)), eps);
					break;
				case RBFType::GAUSSIAN:
					D(i, j) = voxmt::gaussian(voxmt::distance(listepoints.at(i), listepoints.at(j)), eps);
					break;
				default:break;
				}
			}
		}

		Eigen::VectorXd W = D.colPivHouseholderQr().solve(weight);
		return W;
	}

	std::vector<glm::ivec3> interpolation(const std::vector<glm::ivec3>& values, const std::vector<glm::ivec3>& listepoints, RBFType RBF, const float eps, const Eigen::VectorXd& W) {
		std::vector<glm::ivec3> functionValues;
		functionValues.resize(values.size());
		double sum = 0;
		int phi = 0;

		for (int l = 0; l < values.size(); l++) {
			sum = 0;
			functionValues.at(l) = values.at(l);

			for (int k = 0; k < W.size(); k++) {
				switch (RBF) {
				case RBFType::LINEAR:
					phi = voxmt::distance(values.at(l), listepoints.at(k));
					break;
				case RBFType::MULTIQUADRATIC:
					phi = voxmt::multiquadratic(voxmt::distance(values.at(l), listepoints.at(k)), eps);
					break;
				case RBFType::INVERSEQUADRATIC:
					phi = voxmt::inverseQuadratic(voxmt::distance(values.at(l), listepoints.at(k)), eps);
					break;
				case RBFType::INVERSEMULTIQUAD:
					phi = voxmt::inverseMultiquadratic(voxmt::distance(values.at(l), listepoints.at(k)), eps);
					break;
				case RBFType::GAUSSIAN:
					phi = voxmt::gaussian(voxmt::distance(values.at(l), listepoints.at(k)), eps);
					break;
				default: break;
				}
				sum += W[k] * phi;
			}

			functionValues.at(l).z = sum;
		}
		return functionValues;
	}
}
