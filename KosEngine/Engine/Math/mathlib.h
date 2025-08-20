/******************************************************************/
/*!
\file      mathlib.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 28, 2024
\brief     Math functions with reference to CMath

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef MATHLIB_H
#define MATHLIB_H

#include "mathlib.h"
#include <math.h>
#include <cmath>
#include "vector2.h"

#define PI 3.141592653589793238462643383279502884197
namespace mathlibrary {
	class mathlib {
	public:
		//all is done in radian
		//returns in radian
		//x is in radian
		static double funcRadianToDegree(double x);
		static float funcRadianToDegree(float x);

		static double funcDegreeToRadian(double x);
		static float funcDegreeToRadian(float x);

		static double funcCos(double x);
		static float  funcCos(float x);
		static long double funcCos(long double x);

		static double funcSin(double x);
		static float  funcSin(float x);
		static long double funcSin(long double x);

		static double funcTan(double x);
		static float  funcTan(float x);
		static long double funcTan(long double x);

		static double funcArcCos(double x);
		static float funcArcCos(float x);
		static long double funcArcCos(long double x);

		static double funcArcSin(double x);
		static float funcArcSin(float x);
		static long double funcArcSin(long double x);

		static double funcArcTan(double x);
		static float funcArcTan(float x);
		static long double funcArcTan(long double x);

		static double funcArcTan2(double y, double x);
		static float funcArcTan2(float y, float x);
		static long double funcArcTan2(long double y, long double x);


		//power functions
		static double funcPow(double base, double exponent);
		static float funcPow(float base, float exponent);
		static long double funcPow(long double base, long double exponent);

		static double funcSqrt(double base);
		static float funcSqrt(float base);
		static long double funcSqrt(long double base);

		//calculating the hypotenuse of a right angle triangle
		static double funcHypot(double x, double y);
		static float funcHypot(float x, float y);
		static long double funcHypot(long double x, long double y);

		//max and min
		static double funcFmax(double x, double y);
		static float funcFmax(float x, float y);
		static long double funcFmax(long double x, long double y);

		static double funcFmin(double x, double y);
		static float funcFmin(float x, float y);
		static long double funcFmin(long double x, long double y);

		static double funcFabs(double x);
		static float funcFabs(float x);
		static long double funcFabs(long double x);

		static double funcAbs(double x);
		static float funcAbs(float x);
		static long double funcAbs(long double x);

		//boolean
		static bool funcIsGreater(double x, double y);
		static bool funcIsGreater(float x, float y);
		static bool funcIsGreater(long double x, long double y);

		static bool funcIsGreaterEqual(double x, double y);
		static bool funcIsGreaterEqual(float x, float y);
		static bool funcIsGreaterEqual(long double x, long double y);

		static bool funcIsLess(double x, double y);
		static bool funcIsLess(float x, float y);
		static bool funcIsLess(long double x, long double y);

		static bool funcIsLessEqual(double x, double y);
		static bool funcIsLessEqual(float x, float y);
		static bool funcIsLessEqual(long double x, long double y);

		template <typename T, typename U>
		static T Mix(const T& lhs, const T& rhs, const U& a) {
			return lhs * (1 - a) + rhs * a;
		}




	};

}
#endif MATHLIB_H