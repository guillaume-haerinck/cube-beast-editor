#include "rbf.h"

#include <algorithm>
#include <math.h>

namespace voxmt {

	/////////////////////////////////////////////////////////////////////////////
	////////////////////////////// PRIVATE METHODS //////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	float linear(float x) {
		return x;
	}

	float multiquadratic(float x, float eps) {
		return sqrt(1.0f - (eps * x) * (eps * x));
	}

	float inverseQuadratic(float x, float eps) {
		return 1.0f / (1.0f - (eps * x) * (eps * x));
	}

	float inverseMultiquadratic(float x, float eps) {
		return 1.0f / (sqrt(1.0f - (eps * x) * (eps * x)));
	}

	float gaussian(float x, float eps) {
		return exp(-(eps * x) * (eps * x));
	}

	float distance(const glm::ivec3& point1, const glm::ivec3& point2) {
		return static_cast<float>(sqrt((point2.x - point1.x) * (point2.x - point1.x) + (point2.y - point1.y) * (point2.y - point1.y) + (point2.z - point1.z) * (point2.z - point1.z)));
	}

	Eigen::VectorXd vectorWi(const std::vector<glm::ivec3>& controlPointCoords, const Eigen::VectorXd& controlPointWeights, RBFType type, const float eps) {
		Eigen::MatrixXd D(controlPointCoords.size(), controlPointCoords.size());
		
		for (int i = 0; i < D.rows(); i++) {
			for (int j = 0; j < D.cols(); j++) {
				switch (type) {
				case RBFType::LINEAR:
					D(i, j) = distance(controlPointCoords.at(i), controlPointCoords.at(j));
					break;
				case RBFType::MULTIQUADRATIC:
					D(i, j) = multiquadratic(distance(controlPointCoords.at(i), controlPointCoords.at(j)), eps);
					break;
				case RBFType::INVERSEQUADRATIC:
					D(i, j) = inverseQuadratic(distance(controlPointCoords.at(i), controlPointCoords.at(j)), eps);
					break;
				case RBFType::INVERSEMULTIQUAD:
					D(i, j) = inverseMultiquadratic(distance(controlPointCoords.at(i), controlPointCoords.at(j)), eps);
					break;
				case RBFType::GAUSSIAN:
					D(i, j) = gaussian(distance(controlPointCoords.at(i), controlPointCoords.at(j)), eps);
					break;
				default:break;
				}
			}
		}

		return D.colPivHouseholderQr().solve(controlPointWeights);;
	}

	/////////////////////////////////////////////////////////////////////////////
	////////////////////////////// PUBLIC METHODS ///////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	void rbfInterpolate(std::vector<glm::ivec3>& coordXYwheretoGetZ, const std::vector<glm::ivec3>& controlPointCoords, const Eigen::VectorXd& controlPointWeights, RBFType type, const float epsilon) {
		const Eigen::VectorXd& W = vectorWi(controlPointCoords, controlPointWeights, type, epsilon);

		for (int l = 0; l < coordXYwheretoGetZ.size(); l++) {
			double sum = 0;
			float phi = 0;

			for (int k = 0; k < W.size(); k++) {
				switch (type) {
				case RBFType::LINEAR:
					phi = distance(coordXYwheretoGetZ.at(l), controlPointCoords.at(k));
					break;
				case RBFType::MULTIQUADRATIC:
					phi = multiquadratic(distance(coordXYwheretoGetZ.at(l), controlPointCoords.at(k)), epsilon);
					break;
				case RBFType::INVERSEQUADRATIC:
					phi = inverseQuadratic(distance(coordXYwheretoGetZ.at(l), controlPointCoords.at(k)), epsilon);
					break;
				case RBFType::INVERSEMULTIQUAD:
					phi = inverseMultiquadratic(distance(coordXYwheretoGetZ.at(l), controlPointCoords.at(k)), epsilon);
					break;
				case RBFType::GAUSSIAN:
					phi = gaussian(distance(coordXYwheretoGetZ.at(l), controlPointCoords.at(k)), epsilon);
					break;
				default: break;
				}
	
				sum += W[k] * phi;
			}
			coordXYwheretoGetZ.at(l).z = static_cast<int>(sum);
		}
	}
}
