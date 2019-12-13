#include "rbf.h"
#include <glm/glm.hpp>
#include <algorithm>
#include <math.h>

namespace voxmt {
	unsigned int linear(int x){
		return x;
	}

	unsigned int multiquadratic(int x, float eps){
		return (sqrt(1-(eps*x)*(eps*x)));
	}

	unsigned int inverseQuadratic(int x, float eps){
		return 1/(1-(eps*x)*(eps*x));
	}

	unsigned int inverseMultiquadratic(int x, float eps){
		return 1/(sqrt(1-(eps*x)*(eps*x)));
	}

	unsigned int gaussian(int x, float eps){
		return exp(-(eps*x)*(eps*x));
	}

	unsigned int distance(const glm::ivec3 point1, const glm::ivec3 point2) {
		return sqrt((point2[1] - point1[1]) * (point2[1] - point1[1]) + (point2[2] - point1[2]) * (point2[2] - point1[2]) + (point2[3] - point1[3]) * (point2[3] - point1[3]));

	}

/*
	Eigen::VectorXd VectorWi( const Eigen::VectorXd &weight, const glm::ivec3 &listepoints, const std::string &RBF, const float eps) {
		
		Eigen::MatrixXd D(glm::length(listepoints), glm::length(listepoints));

		for (int i=0; i< glm::length(listepoints); i++){
			for(int j=0; j< glm::length(listepoints);j++){
				switch (std::string RBF) {
					case "linear" :
						D[i][j] = distance(listepoints[i], listepoints[j]);

					case "multiquadratic" :
						D[i][j] = multiquadratic(distance(listepoints[i], listepoints[j]), eps);

					case 'inverseQuadratic' :
						D[i][j] = inverseQuadratic(distance(listepoints[i], listepoints[j]), eps);

					case  'inverseMultiquadratic' :
						D[i][j] = inverseMultiquadratic(distance(listepoints[i], listepoints[j]), eps);

					case 'gaussian' :
						D[i][j] = gaussian(distance(listepoints[i], listepoints[j]), eps);

				}


			}


		}

		Eigen::VectorXd W = D.colPivHouseholderQr().solve(weight);
		return W ;
	}

	glm::ivec3 interpolation(const glm::ivec3 &values, const Point3D &listepoints, const std::string &RBF, const float eps, const Eigen::VectorXd W){

		glm::ivec3 FunctionValues (values);
		int sum=0;
		int phi ;
		for (int l=0 ; l< glm::length(values); l++){
			sum=0;
			FunctionValues[l][1] = values[l][1];
			FunctionValues[l][2] = values[l][2];

			for(int k=0;k<W.size();k++){
				switch (std::string RBF) {
					case "linear" :
						phi = distance(values[l], listepoints[k]);

					case "multiquadratic" :
						phi = multiquadratic(distance(values[l], listepoints[k]), eps);

					case 'inverseQuadratic' :
						phi = inverseQuadratic(distance(values[l], listepoints[k]), eps);

					case  'inverseMultiquadratic' :
						phi = inverseMultiquadratic(distance(values[l], listepoints[k]), eps);

					case 'gaussian' :
						phi = gaussian(distance(values[l], listepoints[k]), eps);
					}
				sum += W[k]*phi ;
				}
				

			FunctionValues[l][3] = sum ;
		}
		return FunctionValues;

	}
	*/
}
