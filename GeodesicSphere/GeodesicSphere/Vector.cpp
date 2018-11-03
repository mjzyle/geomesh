#define _CRT_SECURE_NO_WARNINGS

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

class Vector {

public:

	double x, y, z;
	double mag;

	Vector() {

	}

	Vector(double X, double Y, double Z) {
		x = X;
		y = Y;
		z = Z;
		mag = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	void scale(double k) {
		x *= k;
		y *= k;
		z *= k;
	}

	void add(Vector v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}

	bool equals(Vector v) {
		return x == v.x && y == v.y && z == v.z;
	}

};

#endif