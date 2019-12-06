#ifndef 3DPOINT
#define 3DPOINT

#include <iostream>
#include <string> 
#include <algorithm>
#include <vector>
#include <math.h> 
#include<Eigen/Dense>


class Point3D{
	protected :
		uint coord_x = 0 ;
		uint coord_y = 0 ;
		uint coord_z = 0 ;

	public : 
		Point3D() = default;
		Point3D(const double l,const double h);
		~Point3D() = default;
		double & coord_x();
		const double & coord_x() const;
		double & coord_y();
		const double & coord_y() const;
		double & coord_z();
		const double & coord_z() const;

		uint distance(const Point3D X1, const Point3D X2);


}



#endif 