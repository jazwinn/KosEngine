/*!
\file      Mat2x2.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the class for a Matrix of 2x2 size, it's member functions and non member functions along with operator overloads

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma warning (disable : 4201)
#pragma warning(disable : 26495)
#ifndef MAT2x2_H
#define MAT2x2_H
#include <algorithm>

namespace mat2x2 {
	class Mat2x2 {
	public:
		//union for shared memory space among the variables
		union {
			struct {
				float m_e00, m_e01;
				float m_e10, m_e11;
			};
			[[maybe_unused]] float m_mat[2][2];
			[[maybe_unused]] float m_row[4];
		};

		/**
		\brief default contstructor for Mat2x2 class
		*/
		Mat2x2() : m_e00(0.f), m_e10(0.f), m_e01(0.f), m_e11(0.f) {}

		/**
		\brief contstructor for Mat2x2 class
		*/
		Mat2x2(float e1, float e2, float e3, float e4) : m_e00{ e1 }, m_e10{ e2 }, m_e01{ e3 }, m_e11{ e4 } {}

		/*Mat2x2<T> identity(T val) const{
			return Mat2x2<T>(val, 0, 0, val);
		}*/

		/**
		 \brief		this function swaps the data of 2 matrixes
		 **/
		void m_Mat2Swap(Mat2x2& lhs, Mat2x2& rhs);

		/**
		 \brief		This function is a member function meant for assigning a transposed version of the callee
		 **/
		Mat2x2 m_Mat2Transpose() const;

		/**
		 \brief		this function is a member function meant for assigning a fully positive version of the callee
		 **/
		Mat2x2 m_Mat2Abs() const;

		/**
		 \brief		this function is a member function meant for assigning an adjunct version of the callee
		 **/
		Mat2x2 m_Mat2Adjunct() const;

		/**
		 \brief		this function is a member function meant for getting the determinant of the callee
		 **/
		float m_Mat2Determinant() const;

		/**
		 \brief		this function is a member function meant for assigning an inverse version of the callee
		 **/
		Mat2x2 m_Mat2Inverse() const;

		/**
		 \brief		this function is a member operator overload for the = sign also known as the copy assignment
		 **/
		Mat2x2& operator=(const Mat2x2& rhs);

		/**
		 \brief		this function is a member operator overload ofthe *= sign used to multiply a Mat2x2 with the callee
		 **/
		Mat2x2& operator*=(const Mat2x2& rhs);

		/**
		 \brief		this function is a member operator overload of the += sign used to add another matrix with the callee
		 **/
		Mat2x2& operator +=(const Mat2x2& rhs);

		/**
		 \brief		this function is a member operator overload of the -= sign used to subtract another matrix with the callee
		 **/
		Mat2x2& operator -=(const Mat2x2& rhs);
	};

	/**
	 \brief		this function is a operator overload of the * sign used to multiply 2 matrixes together without changing the matrixes themselves
	 **/
	Mat2x2 operator*(const Mat2x2& lhs, const Mat2x2& rhs);

	/**
	 \brief		this function is a operator overload of the + sign used to add 2 matrixes together without changing the matrixes themselves
	 **/
	Mat2x2 operator+(const Mat2x2& lhs, const Mat2x2& rhs);

	/**
	 \brief		this function is a operator overload of the - sign used to subtract 1 matrix from another without changing the matrixes themselves
	 **/
	Mat2x2 operator-(const Mat2x2& lhs, const Mat2x2& rhs);

	/**
	 \brief		this function is a operator overload of the == sign used to compare 2 matrixes and return true if they are equal
	 **/
	bool operator==(const Mat2x2& lhs, const Mat2x2& rhs);

	/**
	 \brief		this function is a operator overload of the >= sign used to compare 2 matrixes and return true if the left matrix is more than or equal to the right matrix
	 **/
	bool operator>=(const Mat2x2& lhs, const Mat2x2& rhs);

	/**
	 \brief		this function is a operator overload of the <= sign used to compare 2 matrixes and return true if the left matrix is less than or equal to the right matrix
	 **/
	bool operator<=(const Mat2x2& lhs, const Mat2x2& rhs);
}


#endif

