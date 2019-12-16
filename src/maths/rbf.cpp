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

	unsigned int distance( glm::ivec3 point1, glm::ivec3 point2) {
		return sqrt((point2[1] - point1[1]) * (point2[1] - point1[1]) + (point2[2] - point1[2]) * (point2[2] - point1[2]) + (point2[3] - point1[3]) * (point2[3] - point1[3]));

	}
}
	typedef enum rbf { LINEAR = 1, MULTIQUADRATIC = 2, INVERSEQUADRATIC = 3, INVERSEMULTIQUAD = 4, GAUSSIAN = 5 } rbf;
	

	Eigen::VectorXd VectorWi( const Eigen::VectorXd &weight,  std::vector<glm::ivec3> &listepoints,rbf RBF, const float eps) {
		
		Eigen::MatrixXd D(listepoints.size(),listepoints.size());
		enum rbf Rbf ;
		Rbf = RBF;
		for (int i=0; i< listepoints.size(); i++){
			for(int j=0; j<listepoints.size();j++){
				switch (Rbf) {
					case LINEAR :
						D(i,j) = voxmt::distance(listepoints[i], listepoints[j]);

					case MULTIQUADRATIC :
						D(i,j) = voxmt::multiquadratic(voxmt::distance(listepoints[i], listepoints[j]), eps);

					case INVERSEQUADRATIC :
						D(i,j) = voxmt::inverseQuadratic(voxmt::distance(listepoints[i], listepoints[j]), eps);

					case  INVERSEMULTIQUAD :
						D(i,j) = voxmt::inverseMultiquadratic(voxmt::distance(listepoints[i], listepoints[j]), eps);

					case GAUSSIAN :
						D(i,j) = voxmt::gaussian(voxmt::distance(listepoints[i], listepoints[j]), eps);

				}


			}


		}

		Eigen::VectorXd W = D.colPivHouseholderQr().solve(weight);
		return W ;
	}
	std::vector<glm::ivec3> interpolation( std::vector<glm::ivec3> &values,  std::vector<glm::ivec3> &listepoints, rbf RBF, const float eps, const Eigen::VectorXd W){

		std::vector<glm::ivec3> FunctionValues (values.size());
		double sum=0;
		int phi ;
		for (int l=0 ; l<values.size(); l++){
			sum=0;
			FunctionValues[l] = values[l];
		
			enum rbf Rbf = RBF;
			for(int k=0;k<W.size();k++){
				switch (Rbf) {
					case LINEAR :
						phi = voxmt::distance(values[l], listepoints[k]);

					case MULTIQUADRATIC :
						phi = voxmt::multiquadratic(voxmt::distance(values[l], listepoints[k]), eps);

					case INVERSEQUADRATIC :
						phi = voxmt::inverseQuadratic(voxmt::distance(values[l], listepoints[k]), eps);

					case  INVERSEMULTIQUAD :
						phi = voxmt::inverseMultiquadratic(voxmt::distance(values[l], listepoints[k]), eps);

					case GAUSSIAN:
						phi = voxmt::gaussian(voxmt::distance(values[l], listepoints[k]), eps);
					}
				sum += W[k]*phi ;
				}
				

			FunctionValues[l][3] = sum ;
		}
		return FunctionValues;

	}
	

