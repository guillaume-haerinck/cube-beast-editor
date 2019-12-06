
#ifndef RBF
#define RBF

#include <iostream>
#include <string> 
#include <algorithm>
#include <vector>
#include <math.h> 
#include<Eigen/Dense>
#include"3dPoint.hpp"



uint linear(int x);
uint multiquadratic(int x, float eps);
uint inverseQuadratic(int x, float eps);
uint inverseMultiquadratic(int x, float eps);
uint gaussian(int x, float eps); 

VectorXd VectorWi(const int weight, const Point3D listepoints, const float eps);
VectorXd interpolation(const VectorXd &values, const Point3D &listepoints, const std::string &RBF, const float eps, const VectorXd W);

#endif 