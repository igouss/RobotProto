/*
 *  MatrixMath.h Library for Matrix Math
 *
 *  Created by Charlie Matlack on 12/18/10.
 *  Modified from code by RobH45345 on Arduino Forums, taken from unknown source.
 */

#ifndef MatrixMath_h
#define MatrixMath_h

#include "WProgram.h"

class MatrixMath
{
public:
	static int8_t invert3x3(double matrix[3][3]);
	static double dot(double v1[3], double v2[3]);
	static void transpose3x3(double A[3][3]);
	static void crossProduct(const double v1[3], const double v2[3], double result[3]);
public:
	static double matrix3x3[3][3];
};

#endif
