#define _CRT_SECURE_NO_WARNINGS

#ifndef FACE3_H
#define FACE3_H

#include <iostream>
#include <Eigen/Dense>
#include "Vector.cpp"
using namespace Eigen;

class Face3 {

public:
	
	Vector a, b, c, center, normal;

	Face3() {

	}

	Face3 (Vector A, Vector B, Vector C) {
		a = A;
		b = B;
		c = C;
		find_center();
		find_normal();
	}

	void find_center() {
		Vector ab = Vector(b.x - a.x, b.y - a.y, b.z - a.z);
		Vector bc = Vector(c.x - b.x, c.y - a.y, c.z - a.z);
		Vector ac = Vector(c.x - a.z, c.y - a.y, c.z - a.z);
		ab.scale(1 / 2);
		bc.scale(1 / 2);
		ac.scale(1 / 2);
		ab.add(a);
		bc.add(b);
		ac.add(a);

		Vector A = Vector(bc.x - a.x, bc.y - a.y, bc.z - a.z);
		Vector B = Vector(ac.x - b.x, ac.y - b.y, ac.z - b.z);

		MatrixXd I(5, 5);
		I << 1, 0, 0, -A.x, 0,
			 1, 0, 0, 0,    -B.x,
			 0, 1, 0, -A.y, 0,
			 0, 1, 0, 0,    -B.y,
			 0, 0, 1, -A.z, 0;
			 
		MatrixXd J(5, 1);
		J << a.x, 
			 b.x, 
			 a.y, 
			 b.y, 
			 a.z;

		MatrixXd D(5, 1);
		D = I.inverse() * J;

		center.x = D(0, 0);
		center.y = D(1, 0);
		center.z = D(2, 0);
	}

	void find_normal() {
		Vector ab = Vector(b.x - a.x, b.y - a.y, b.z - a.z);
		Vector ac = Vector(c.x - a.z, c.y - a.y, c.z - a.z);

		Vector3f x(ab.x, ab.y, ab.z);
		Vector3f y(ac.x, ac.y, ac.z);

		Vector3f n;
		n = x.cross(y);

		normal = Vector(n(0), n(1), n(2));
		normal.scale(1 / normal.mag);
	}

	void flip_normal() {
		normal.x *= -1;
		normal.y *= -1;
		normal.z *= -1;
	}

};

#endif