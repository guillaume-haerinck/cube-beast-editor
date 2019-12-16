#include "rbf.h"
#include <glm/glm.hpp>
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

	unsigned int distance(glm::ivec3 point1, glm::ivec3 point2) {
		return sqrt((point2[1] - point1[1]) * (point2[1] - point1[1]) + (point2[2] - point1[2]) * (point2[2] - point1[2]) + (point2[3] - point1[3]) * (point2[3] - point1[3]));

	}


	Eigen::VectorXd VectorWi(const Eigen::VectorXd& weight, std::vector<glm::ivec3>& listepoints, RBFType RBF, const float eps) {

		Eigen::MatrixXd D(listepoints.size(), listepoints.size());
		for (int i = 0; i < listepoints.size(); i++) {
			for (int j = 0; j < listepoints.size();j++) {
				switch (RBF) {
				case RBFType::LINEAR:
					D(i, j) = voxmt::distance(listepoints[i], listepoints[j]);

				case RBFType::MULTIQUADRATIC:
					D(i, j) = voxmt::multiquadratic(voxmt::distance(listepoints[i], listepoints[j]), eps);

				case RBFType::INVERSEQUADRATIC:
					D(i, j) = voxmt::inverseQuadratic(voxmt::distance(listepoints[i], listepoints[j]), eps);

				case  RBFType::INVERSEMULTIQUAD:
					D(i, j) = voxmt::inverseMultiquadratic(voxmt::distance(listepoints[i], listepoints[j]), eps);

				case RBFType::GAUSSIAN:
					D(i, j) = voxmt::gaussian(voxmt::distance(listepoints[i], listepoints[j]), eps);

				}


			}


		}

		Eigen::VectorXd W = D.colPivHouseholderQr().solve(weight);
		return W;
	}
	std::vector<glm::ivec3> interpolation(std::vector<glm::ivec3>& values, std::vector<glm::ivec3>& listepoints, RBFType RBF, const float eps, const Eigen::VectorXd W) {

		std::vector<glm::ivec3> FunctionValues(values.size());
		double sum = 0;
		int phi;
		for (int l = 0; l < values.size(); l++) {
			sum = 0;
			FunctionValues[l] = values[l];

			
			for (int k = 0;k < W.size();k++) {
				switch (RBF) {
				case RBFType::LINEAR:
					phi = voxmt::distance(values[l], listepoints[k]);

				case RBFType::MULTIQUADRATIC:
					phi = voxmt::multiquadratic(voxmt::distance(values[l], listepoints[k]), eps);

				case RBFType::INVERSEQUADRATIC:
					phi = voxmt::inverseQuadratic(voxmt::distance(values[l], listepoints[k]), eps);

				case  RBFType::INVERSEMULTIQUAD:
					phi = voxmt::inverseMultiquadratic(voxmt::distance(values[l], listepoints[k]), eps);

				case RBFType::GAUSSIAN:
					phi = voxmt::gaussian(voxmt::distance(values[l], listepoints[k]), eps);
				}
				sum += W[k] * phi;
			}


			FunctionValues[l][3] = sum;
		}
		return FunctionValues;

	}

}
