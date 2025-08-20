/*!
\file      Mat3x3.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the class for a Matrix of 3x3 size, it's member functions and non member functions along with operator overloads

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma warning (disable : 4201)
#pragma warning(disable : 26495)
#ifndef MAT3x3_H
#define MAT3x3_H
#include <algorithm>
#include <math.h>
#include "../Math/vector2.h"

#include "vector2.h"



namespace mat3x3 {
	constexpr float MAT_PI = 3.14159265359f;
	class Mat3x3 {
	public:
		union {
			struct {
				float m_e00, m_e01, m_e02;	//e00 e10 e20
				float m_e10, m_e11, m_e12;	//e01 e11 e21
				float m_e20, m_e21, m_e22;	//e02 e12 e22
			};
			[[maybe_unused]]float m_Mat[3][3];
			[[maybe_unused]] float m_Row[9];
		};

		Mat3x3() : m_e00(0.f), m_e01(0.f), m_e02(0.f), m_e10(0.f), m_e11(0.f), m_e12(0.f), m_e20(0.f), m_e21(0.f), m_e22(0.f) {}

		Mat3x3(float e1, float e2, float e3, float e4, float e5, float e6, float e7, float e8, float e9) :
			m_e00{ e1 }, m_e10{ e4 }, m_e20{ e7 },
			m_e01{ e2 }, m_e11{ e5 }, m_e21{ e8 },
			m_e02{ e3 }, m_e12{ e6 }, m_e22{ e9 } {}

		/**
		 \brief		this function swaps the data of 2 matrixes
		 **/
		void m_Mat3Swap(Mat3x3& lhs, Mat3x3& rhs);

		/**
		 \brief		this function is a member function meant for assigning an adjunct version of the callee
		 **/
		Mat3x3 m_Mat3Adjunct() const;

		/**
		 \brief		this function is a member function meant for getting the determinant of the callee
		 **/
		float m_Mat3Determinant() const;

		/**
		 \brief		this function is a member function overload for the = sign also known as the copy assignment
		 **/
		Mat3x3& operator=(const Mat3x3& rhs);

		/**
		 \brief		this function is a member function overload ofthe *= sign used to multiply a Mat2x2 with the callee
		 **/
		Mat3x3& operator*=(const Mat3x3& rhs);

		vector2::Vec2& operator*=(vector2::Vec2& rhs) const;

		/**
		 \brief		this function is a member function overload of the += sign used to add another matrix with the callee
		 **/
		Mat3x3& operator+=(const Mat3x3& rhs);

		/**
		 \brief		this function is a member function overload of the -= sign used to subtract another matrix with the callee
		 **/
		Mat3x3& operator-=(const Mat3x3& rhs);

		/**
		 \brief		this function is a member function meant for assigning a fully positive version of the callee
		 **/
		Mat3x3 m_Mat3Abs() const;


	};

	void Mat3Decompose(const Mat3x3& in, vector2::Vec2& translation, vector2::Vec2& scale, float& rotate);

	/**
	\brief		this function is a meant for changing a given Matrix into an identity matrix
	**/
	void Mat3Identity(Mat3x3& given);

	/**
	 \brief		this function is a meant for inversing a given matrix and changing the value of the given resultant matrix
	 **/
	void Mat3Inverse(const Mat3x3& givenIn, Mat3x3& givenOut);

	/**
	 \brief		this function is a meant for changing a given Matrix into a translation matrix with x and y
	 **/
	void Mat3Translate(Mat3x3& given, float x, float y);

	/**
	 \brief		this function is a meant for changing a given Matrix into a scale matrix with x and y
	 **/
	void Mat3Scale(Mat3x3& given, float x, float y);

	/**
	 \brief		this function is a meant for changing a given Matrix into a rotation matrix with a given angle in radian
	 **/
	void Mat3RotRad(Mat3x3& given, float ang);

	/**
	 \brief		this function is a meant for changing a given Matrix into a rotation matrix by converting an angle in degree to radian
	 **/
	void Mat3RotDeg(Mat3x3& given, float ang);

	Mat3x3 Mat3Transform(vector2::Vec2 translate, vector2::Vec2 scale, float rotate);

	/**
	 \brief		this is a operator overload of the * sign used to multiply 2 matrixes together without changing the matrixes themselves
	 **/
	Mat3x3 operator*(const Mat3x3& lhs, const Mat3x3& rhs);

	vector2::Vec2 operator*(const Mat3x3& lhs, const vector2::Vec2& rhs);

	/**
	 \brief		this function is a operator overload of the + sign used to add 2 matrixes together without changing the matrixes themselves
	 **/
	Mat3x3 operator+(const Mat3x3& lhs, const Mat3x3& rhs);

	/**
	 \brief		this function is a operator overload of the - sign used to subtract 1 matrix from another without changing the matrixes themselves
	 **/
	Mat3x3 operator-(const Mat3x3& lhs, const Mat3x3& rhs);

	/**
	 \brief		this function is a operator overload of the == sign used to compare 2 matrixes and return true if they are equal
	 **/
	bool operator==(const Mat3x3& lhs, const Mat3x3& rhs);

	/**
	 \brief		this function is a operator overload of the <= sign used to compare 2 matrixes and return true if
				any left matrix element is smaller than or equal to right matrix
	 **/
	bool operator<=(const Mat3x3& lhs, const Mat3x3& rhs);

	/**
	 \brief		this function is a operator overload of the >= sign used to compare 2 matrixes and return true if
				any left matrix element is bigger than or equal to right matrix
	 **/
	bool operator>=(const Mat3x3& lhs, const Mat3x3& rhs);
}
#endif