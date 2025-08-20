/******************************************************************/
/*!
\file      mathlib.cpp
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 28, 2024
\brief     Math Library with reference to CMath

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"
#include "Mathlib.h"


namespace mathlibrary {
	double mathlib::funcRadianToDegree(double x) { return x * (180.0 / PI); }
	float  mathlib::funcRadianToDegree(float x) { return static_cast<float>(x * (180.0f / PI)); }

	double  mathlib::funcDegreeToRadian(double x) { return x * (PI / -180.0); }
	float mathlib::funcDegreeToRadian(float x) { return static_cast<float>(x * (PI / -180.0f)); }


	double mathlib::funcCos(double x) { return cos(x); }
	float  mathlib::funcCos(float x) { return cos(x); }
	long double mathlib::funcCos(long double x) { return cos(x); }

	double mathlib::funcSin(double x) { return sin(x); }
	float  mathlib::funcSin(float x) { return sin(x); }
	long double mathlib::funcSin(long double x) { return sin(x); }

	double mathlib::funcTan(double x) { return tan(x); }
	float  mathlib::funcTan(float x) { return tan(x); }
	long double mathlib::funcTan(long double x) { return tan(x); }

	double mathlib::funcArcCos(double x) { return acos(x); }
	float mathlib::funcArcCos(float x) { return acos(x); }
	long double mathlib::funcArcCos(long double x) { return acos(x); }

	double mathlib::funcArcSin(double x) { return asin(x); }
	float mathlib::funcArcSin(float x) { return asin(x); }
	long double mathlib::funcArcSin(long double x) { return asin(x); }

	double mathlib::funcArcTan(double x) { return atan(x); }
	float mathlib::funcArcTan(float x) { return atan(x); }
	long double mathlib::funcArcTan(long double x) { return atan(x); }

	double mathlib::funcArcTan2(double y, double x) { return atan2(y, x); }
	float mathlib::funcArcTan2(float y, float x) { return atan2(y, x); }
	long double mathlib::funcArcTan2(long double y, long double x) { return atan2(y, x); }


	//power functions
	double mathlib::funcPow(double base, double exponent) { return pow(base, exponent); }
	float mathlib::funcPow(float base, float exponent) { return pow(base, exponent); }
	long double mathlib::funcPow(long double base, long double exponent) { return pow(base, exponent); }

	double mathlib::funcSqrt(double base) { return sqrt(base); }
	float mathlib::funcSqrt(float base) { return sqrt(base); }
	long double mathlib::funcSqrt(long double base) { return sqrt(base); }

	//calculating the hypotenuse of a right angle triangle
	double mathlib::funcHypot(double x, double y) { return hypot(x, y); }
	float mathlib::funcHypot(float x, float y) { return hypot(x, y); }
	long double mathlib::funcHypot(long double x, long double y) { return hypot(x, y); }

	//max and min
	double mathlib::funcFmax(double x, double y) { return fmax(x, y); }
	float mathlib::funcFmax(float x, float y) { return fmax(x, y); }
	long double mathlib::funcFmax(long double x, long double y) { return fmax(x, y); }

	double mathlib::funcFmin(double x, double y) { return fmin(x, y); }
	float mathlib::funcFmin(float x, float y) { return fmin(x, y); }
	long double mathlib::funcFmin(long double x, long double y) { return fmin(x, y); }

	double mathlib::funcFabs(double x) { return fabs(x); }
	float mathlib::funcFabs(float x) { return fabs(x); }
	long double mathlib::funcFabs(long double x) { return fabs(x); }

	double mathlib::funcAbs(double x) { return abs(x); }
	float mathlib::funcAbs(float x) { return abs(x); }
	long double mathlib::funcAbs(long double x) { return abs(x); }

	//boolean
	bool mathlib::funcIsGreater(double x, double y) { return isgreater(x, y); }
	bool mathlib::funcIsGreater(float x, float y) { return isgreater(x, y); }
	bool mathlib::funcIsGreater(long double x, long double y) { return isgreater(x, y); }

	bool mathlib::funcIsGreaterEqual(double x, double y) { return isgreaterequal(x, y); }
	bool mathlib::funcIsGreaterEqual(float x, float y) { return isgreaterequal(x, y); }
	bool mathlib::funcIsGreaterEqual(long double x, long double y) { return isgreaterequal(x, y); }

	bool mathlib::funcIsLess(double x, double y) { return isless(x, y); }
	bool mathlib::funcIsLess(float x, float y) { return isless(x, y); }
	bool mathlib::funcIsLess(long double x, long double y) { return isless(x, y); }

	bool mathlib::funcIsLessEqual(double x, double y) { return islessequal(x, y); }
	bool mathlib::funcIsLessEqual(float x, float y) { return islessequal(x, y); }
	bool mathlib::funcIsLessEqual(long double x, long double y) { return islessequal(x, y); }

}

