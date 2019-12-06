#include <iostream>
#include <string> 
#include <algorithm>
#include <vector>
#include <math.h> 
#include<Eigen/Dense>
#include"3dPoint.hpp"



Point3D::Point3D(const uint coord_x , const uint y, const uint z): coord_x(x), coord_y(y), coord_z(z){}



const uint &Point3D::coord_x(){return coord_x;}

const uint & Point3D::coord_y()const{return coord_y;}
const uint & Point3D::coord_z()const{return coord_z;}


uint distance(const Point3D X1, const Point3D X2){
	uint distance = sqrt(pow((X1.coord_x - X2.coord_x),2) + pow((X1.coord_y -X2.coord_y),2) + pow((X1.coord_z - X2.coord_z),2))
	return distance;

}
