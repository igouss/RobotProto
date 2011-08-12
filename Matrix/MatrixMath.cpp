/*
 *  MatrixMath.cpp Library for MatrixMath
 *
 *  Created by Charlie Matlack on 12/18/10.
 *  Modified from code by RobH45345 on Arduino Forums, taken from unknown source.
 *  MatrixMath.cpp
 */

#include "WProgram.h"
#include "MatrixMath.h"

// row1
#define a A[0][0]
#define b A[0][1]
#define c A[0][2]
// row2
#define d A[1][0]
#define e A[1][1]
#define f A[1][2]
// row3
#define g A[2][0]
#define h A[2][1]
#define k A[2][2]

/* Inversion of 3×3 matrices.
 * Result is stored in MatrixMath::matrix3x3
 *
 * @see http://en.wikipedia.org/wiki/Invertible_matrix#Inversion_of_3.C3.973_matrices
 *
 * A = | a b c |
 *     | d e f |
 *     | g h k |
 * inv(A) = 1/det(A) * | A D G |
 *                     | B E H |
 *                     | C F K |
 * det(A) =  a(e*k-f*h) + b(f*g-k*d) + c(d*h-e*g)
 */
int8_t MatrixMath::invert3x3(double A[3][3]) {
	double determinant = a * (e * k - f * h) + b * (f * g - k * d) + c * (d * h	- e * g);
	if (determinant != 0) { // invertable
		double invdet = 1 / determinant;
		MatrixMath::matrix3x3[0][0] = (e * k - f * h) * invdet; // A = ek-fh
		MatrixMath::matrix3x3[0][1] = (c * h - b * k) * invdet; // D = ch-bk
		MatrixMath::matrix3x3[0][2] = (b * f - c * e) * invdet; // G = bf-ce
		MatrixMath::matrix3x3[1][0] = (f * g - d * k) * invdet; // B = fg-dk
		MatrixMath::matrix3x3[1][1] = (a * k - c * g) * invdet; // E = ak-cg
		MatrixMath::matrix3x3[1][2] = (c * d - a * f) * invdet; // H = cd-af
		MatrixMath::matrix3x3[2][0] = (d * h - e * g) * invdet; // C = dh-eg
		MatrixMath::matrix3x3[2][1] = (g * b - a * h) * invdet; // F = gb-ah
		MatrixMath::matrix3x3[2][2] = (a * e - b * d) * invdet; // K = ae-bd
		return 1;
	} else {
		return 0;
	}
}

/**
 *
 */
double MatrixMath::dot(double v1[3], double v2[3]) {
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

/**
 * The cross product takes two vectors and returns a perpendicular vector to the plane defined by the two vectors.
 * Assuming that v1 and v2 are in the XZ plane, then the direction of the cross product vector will point upwards
 * if the rotation from v1 to v2 is counter clockwise, and downwards if the rotation is clockwise.
 * Its usage includes, amongst other things, lighting computations, see the terrain tutorial, or to find the angle
 * between vectors.
 *
 * @param v1[3] vector 1
 * @param v2[3] vector 2
 * @param result[3] result vector
 */
void MatrixMath::crossProduct(const double v1[3], const double v2[3], double result[3]) {
	result[0] = v1[1] * v2[2] - v2[1] * v1[2];
	result[1] = v1[2] * v2[0] - v2[2] * v1[0];
	result[2] = v1[0] * v2[1] - v2[0] * v1[1];
}

void MatrixMath::transpose3x3(double A[3][3]) {
	MatrixMath::matrix3x3[0][0] = a;
	MatrixMath::matrix3x3[0][1] = d;
	MatrixMath::matrix3x3[0][2] = g;
	MatrixMath::matrix3x3[1][0] = b;
	MatrixMath::matrix3x3[1][1] = e;
	MatrixMath::matrix3x3[1][2] = h;
	MatrixMath::matrix3x3[2][0] = c;
	MatrixMath::matrix3x3[2][1] = f;
	MatrixMath::matrix3x3[2][2] = k;
}

// row1
#undef a
#undef b
#undef c
// row2
#undef d
#undef e
#undef f
// row3
#undef g
#undef h
#undef k
