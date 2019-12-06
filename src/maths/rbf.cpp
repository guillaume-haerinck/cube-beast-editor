#include "rbf.h"

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

/*
	VectorXd VectorWi( const VectorXd &weight, const Point3D &listepoints, const std::string &RBF, const float eps) {
		
		MatrixXd D(listepoints.size(), listepoints.size());

		for (int i=0; i< listepoints.size(); i++){
			for(int j=0; j<listepoints.size();j++){
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

		VectorXd W = D.colPivHouseholderQr().solve(weight);
		return W ;
	}

	VectorXd interpolation(const VectorXd &values, const Point3D &listepoints, const std::string &RBF, const float eps, const VectorXd W){

		VectorXd FunctionValues (values);
		int sum=0;
		int phi ;
		for (int l=0 ; l<values.size(); l++){
			sum=0;

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
				

			FunctionValues[l] = sum ;
		}
		return FunctionValues;

	}
	*/
}
