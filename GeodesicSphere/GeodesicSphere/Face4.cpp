#define _CRT_SECURE_NO_WARNINGS

#ifndef FACE4_H
#define FACE4_H

#include <iostream>
#include "Vector.cpp"
#include "Face3.cpp"

class Face4: public Face3 {

public:

	Vector a, b, c, d, center, normal;

	Face4() {

	}

	Face4(Vector A, Vector B, Vector C, Vector D) {
		a = A;
		b = B;
		c = C;
		d = D;
		find_center();
		find_normal();
	}

	void find_center() {
		
	}

};

#endif