/*!
\file      Mat3x3.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the definition Matrix of 3x3 size member functions and non member functions along with operator overloads

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"
#include "../Math/Mat3x3.h"

namespace mat3x3{

	void Mat3x3::m_Mat3Swap(Mat3x3& lhs, Mat3x3 & rhs) {
		Mat3x3 temp(lhs);
		lhs.m_e00 = rhs.m_e00;
		lhs.m_e01 = rhs.m_e01;
		lhs.m_e02 = rhs.m_e02;

		lhs.m_e10 = rhs.m_e10;
		lhs.m_e11 = rhs.m_e11;
		lhs.m_e12 = rhs.m_e12;

		lhs.m_e20 = rhs.m_e20;
		lhs.m_e21 = rhs.m_e21;
		lhs.m_e22 = rhs.m_e22;

		rhs.m_e00 = temp.m_e00;
		rhs.m_e01 = temp.m_e01;
		rhs.m_e02 = temp.m_e02;

		rhs.m_e10 = temp.m_e10;
		rhs.m_e11 = temp.m_e11;
		rhs.m_e12 = temp.m_e12;

		rhs.m_e20 = temp.m_e20;
		rhs.m_e21 = temp.m_e21;
		rhs.m_e22 = temp.m_e22;
	}


	Mat3x3 Mat3x3::m_Mat3Adjunct() const {
		Mat3x3 lvRet;

		lvRet.m_e00 = m_e11 * m_e22 - m_e21 * m_e12;
		lvRet.m_e01 = m_e02 * m_e21 - m_e01 * m_e22;
		lvRet.m_e02 = m_e01 * m_e12 - m_e02 * m_e11;

		lvRet.m_e10 = m_e12 * m_e20 - m_e10 * m_e22;
		lvRet.m_e11 = m_e00 * m_e22 - m_e02 * m_e20;
		lvRet.m_e12 = m_e10 * m_e02 - m_e00 * m_e12;

		lvRet.m_e20 = m_e10 * m_e21 - m_e20 * m_e11;
		lvRet.m_e21 = m_e20 * m_e01 - m_e00 * m_e21;
		lvRet.m_e22 = m_e00 * m_e11 - m_e10 * m_e01;

		return lvRet;
	}


	float Mat3x3::m_Mat3Determinant() const {
		float lvRet;
		lvRet = (m_e00 * (m_e11 * m_e22 - m_e21 * m_e12)) - (m_e10 * (m_e01 * m_e22 - m_e02 * m_e21)) + (m_e20 * (m_e01 * m_e12 - m_e02 * m_e11));
		return lvRet;
	}


	Mat3x3 & Mat3x3::operator=(const Mat3x3 & rhs) {
		Mat3x3 lvTemp(rhs);
		m_Mat3Swap(*this, lvTemp);
		return *this;
	}


	Mat3x3 & Mat3x3::operator*=(const Mat3x3 & rhs) {
		Mat3x3 lvTemp{};
		lvTemp.m_e00 = (m_e00 * rhs.m_e00) + (m_e10 * rhs.m_e01) + (m_e20 * rhs.m_e02);
		lvTemp.m_e01 = (m_e01 * rhs.m_e00) + (m_e11 * rhs.m_e01) + (m_e21 * rhs.m_e02);
		lvTemp.m_e02 = (m_e02 * rhs.m_e00) + (m_e12 * rhs.m_e01) + (m_e22 * rhs.m_e02);

		lvTemp.m_e10 = (m_e00 * rhs.m_e10) + (m_e10 * rhs.m_e11) + (m_e20 * rhs.m_e12);
		lvTemp.m_e11 = (m_e01 * rhs.m_e10) + (m_e11 * rhs.m_e11) + (m_e21 * rhs.m_e12);
		lvTemp.m_e12 = (m_e02 * rhs.m_e10) + (m_e12 * rhs.m_e11) + (m_e22 * rhs.m_e12);

		lvTemp.m_e20 = (m_e00 * rhs.m_e20) + (m_e10 * rhs.m_e21) + (m_e20 * rhs.m_e22);
		lvTemp.m_e21 = (m_e01 * rhs.m_e20) + (m_e11 * rhs.m_e21) + (m_e21 * rhs.m_e22);
		lvTemp.m_e22 = (m_e02 * rhs.m_e20) + (m_e12 * rhs.m_e21) + (m_e22 * rhs.m_e22);
		*this = lvTemp;
		return *this;
	}

	vector2::Vec2& Mat3x3::operator*=(vector2::Vec2& rhs) const{
		vector2::Vec2 lvTemp{ 0,0 };
		lvTemp.m_x = (m_e00 * rhs.m_x) + (m_e10 * rhs.m_y) + (m_e20);
		lvTemp.m_y = (m_e01 * rhs.m_x) + (m_e11 * rhs.m_y) + (m_e21);
		rhs = lvTemp;
		return rhs;
	}


	Mat3x3& Mat3x3::operator+=(const Mat3x3& rhs) {
		m_e00 += rhs.m_e00;
		m_e01 += rhs.m_e01;
		m_e02 += rhs.m_e02;

		m_e10 += rhs.m_e10;
		m_e11 += rhs.m_e11;
		m_e12 += rhs.m_e12;

		m_e20 += rhs.m_e20;
		m_e21 += rhs.m_e21;
		m_e22 += rhs.m_e22;

		return *this;
	}


	Mat3x3& Mat3x3::operator-=(const Mat3x3& rhs) {
		m_e00 -= rhs.m_e00;
		m_e01 -= rhs.m_e01;
		m_e02 -= rhs.m_e02;

		m_e10 -= rhs.m_e10;
		m_e11 -= rhs.m_e11;
		m_e12 -= rhs.m_e12;

		m_e20 -= rhs.m_e20;
		m_e21 -= rhs.m_e21;
		m_e22 -= rhs.m_e22;

		return *this;
	}


	Mat3x3 Mat3x3::m_Mat3Abs() const {
		float lvAb1 = m_e00, lvAb2 = m_e10, lvAb3 = m_e20;
		float lvAb4 = m_e01, lvAb5 = m_e11, lvAb6 = m_e21;
		float lvAb7 = m_e02, lvAb8 = m_e12, lvAb9 = m_e22;
		lvAb1 = lvAb1 < 0.f ? -lvAb1 : lvAb1;
		lvAb2 = lvAb2 < 0.f ? -lvAb2 : lvAb2;
		lvAb3 = lvAb3 < 0.f ? -lvAb3 : lvAb3;
		lvAb4 = lvAb4 < 0.f ? -lvAb4 : lvAb4;
		lvAb5 = lvAb5 < 0.f ? -lvAb5 : lvAb5;
		lvAb6 = lvAb6 < 0.f ? -lvAb6 : lvAb6;
		lvAb7 = lvAb7 < 0.f ? -lvAb7 : lvAb7;
		lvAb8 = lvAb8 < 0.f ? -lvAb8 : lvAb8;
		lvAb9 = lvAb9 < 0.f ? -lvAb9 : lvAb9;
		return Mat3x3(lvAb1,lvAb2,lvAb3,lvAb4,lvAb5,lvAb6,lvAb7,lvAb8,lvAb9);
	}


	void Mat3Decompose(const Mat3x3& matrix, vector2::Vec2& translation, vector2::Vec2& scale, float& rotate)
	{
		translation.m_x = matrix.m_e20;
		translation.m_y = matrix.m_e21;

		scale.m_x = std::sqrt(matrix.m_e00 * matrix.m_e00 + matrix.m_e01 * matrix.m_e01);
		scale.m_y = std::sqrt(matrix.m_e10 * matrix.m_e10 + matrix.m_e11 * matrix.m_e11);

		// Normalize the rotation components
		float norm_e00 = matrix.m_e00 / scale.m_x;
		float norm_e01 = matrix.m_e01 / scale.m_x;
		float norm_e10 = matrix.m_e10 / scale.m_y;
		float norm_e11 = matrix.m_e11 / scale.m_y;
		// Extract raw rotation in radians

		rotate = std::atan2(norm_e01, norm_e00);
		rotate = -rotate * (180.f / MAT_PI);

		if (rotate < 0.0f) {
			rotate += 360.0f;
		}

		// Check if the matrix represents exactly 360 degrees
		if (rotate != 0.0f) {
			if (std::abs(norm_e00 - 1.0f) < 1e-6f && std::abs(norm_e10 - 0.0f) < 1e-6f &&
				std::abs(norm_e01 - 0.0f) < 1e-6f && std::abs(norm_e11 - 1.0f) < 1e-6f) {
				rotate = 0.0f; // Treat as a full rotation
			}
		}
		scale.m_x = std::round(scale.m_x * 100.0f) / 100.0f;
		scale.m_y = std::round(scale.m_y * 100.0f) / 100.0f;
	}

	void Mat3Identity(Mat3x3 & given) {
		given.m_e00 = 1;
		given.m_e01 = 0;
		given.m_e02 = 0;

		given.m_e10 = 0;
		given.m_e11 = 1;
		given.m_e12 = 0;

		given.m_e20 = 0;
		given.m_e21 = 0;
		given.m_e22 = 1;
	}


	void Mat3Inverse(const Mat3x3 & givenIn, Mat3x3 & givenOut) {
		float lvDet = givenIn.m_Mat3Determinant();
		if (lvDet == 0) {
			return;
		}
		float lvMul = 1 / lvDet;

		Mat3x3 lvAdj = givenIn.m_Mat3Adjunct();
		givenOut.m_e00 = lvAdj.m_e00 * lvMul;
		givenOut.m_e01 = lvAdj.m_e01 * lvMul;
		givenOut.m_e02 = lvAdj.m_e02 * lvMul;

		givenOut.m_e10 = lvAdj.m_e10 * lvMul;
		givenOut.m_e11 = lvAdj.m_e11 * lvMul;
		givenOut.m_e12 = lvAdj.m_e12 * lvMul;

		givenOut.m_e20 = lvAdj.m_e20 * lvMul;
		givenOut.m_e21 = lvAdj.m_e21 * lvMul;
		givenOut.m_e22 = lvAdj.m_e22 * lvMul;
	}


	void Mat3Translate(Mat3x3 & given, float x, float y) {
		Mat3Identity(given);
		given.m_e20 = x;
		given.m_e21 = y;
		given.m_e22 = 1;
	}


	void Mat3Scale(Mat3x3 & given, float x, float y) {
		Mat3Identity(given);
		given.m_e00 = x;
		given.m_e11 = y;
		given.m_e22 = 1;
	}



	void Mat3RotRad(Mat3x3 & given, float ang) {
		Mat3Identity(given);
		given.m_e00 = cosf(ang);
		given.m_e10 = sinf(ang);
		given.m_e01 = -sinf(ang);
		given.m_e11 = cosf(ang);
		given.m_e22 = 1;
	}


	void Mat3RotDeg(Mat3x3 & given, float ang) {
		float lvRad = (ang * (MAT_PI / 180.f));
		Mat3RotRad(given, lvRad);
	}

	Mat3x3 Mat3Transform(vector2::Vec2 translate,  vector2::Vec2 scale, float rotate) {
		Mat3x3 matTranslate{}, matRotate{}, matScale{};

		Mat3Translate(matTranslate, translate.m_x, translate.m_y);
		Mat3RotDeg(matRotate, rotate);
		Mat3Scale(matScale, scale.m_x, scale.m_y);

		Mat3x3 transform = matTranslate * (matRotate * matScale);

		return transform;

	}


	Mat3x3 operator*(const Mat3x3 & lhs, const Mat3x3 & rhs) {
		Mat3x3 lvTemp(lhs);
		lvTemp *= rhs;
		return lvTemp;
	}

	vector2::Vec2 operator*(const Mat3x3& lhs, const vector2::Vec2& rhs) {
		vector2::Vec2 lvTemp{ rhs };
		//lhs *= rhs;
		lvTemp = (lhs *= lvTemp);
		return lvTemp;
	}


	Mat3x3 operator+(const Mat3x3 & lhs, const Mat3x3 & rhs) {
		Mat3x3 lvTemp(lhs);
		lvTemp += rhs;
		return lvTemp;
	}


	Mat3x3 operator-(const Mat3x3 & lhs, const Mat3x3 & rhs) {
		Mat3x3 lvTemp(lhs);
		lvTemp -= rhs;
		return lvTemp;
	}


	bool operator==(const Mat3x3& lhs, const Mat3x3& rhs) {
		bool lvMe00 = false, lvMe01 = false, lvMe02 = false;
		bool lvMe10 = false, lvMe11 = false, lvMe12 = false;
		bool lvMe20 = false, lvMe21 = false, lvMe22 = false;
		if (lhs.m_e00 == rhs.m_e00) {
			lvMe00 = true;
		}
		if (lhs.m_e01 == rhs.m_e01) {
			lvMe01 = true;
		}
		if (lhs.m_e02 == rhs.m_e02) {
			lvMe02 = true;
		}
		if (lhs.m_e10 == rhs.m_e10) {
			lvMe10 = true;
		}
		if (lhs.m_e11 == rhs.m_e11) {
			lvMe11 = true;
		}
		if (lhs.m_e12 == rhs.m_e12) {
			lvMe12 = true;
		}
		if (lhs.m_e20 == rhs.m_e20) {
			lvMe20 = true;
		}
		if (lhs.m_e21 == rhs.m_e21) {
			lvMe21 = true;
		}
		if (lhs.m_e22 == rhs.m_e22) {
			lvMe22 = true;
		}

		return lvMe00 && lvMe01 && lvMe02 && lvMe10 && lvMe11 && lvMe12 && lvMe20 && lvMe21 && lvMe22;

	}


	bool operator<=(const Mat3x3& lhs, const Mat3x3& rhs) {
		bool lvMe00 = false, lvMe01 = false, lvMe02 = false;
		bool lvMe10 = false, lvMe11 = false, lvMe12 = false;
		bool lvMe20 = false, lvMe21 = false, lvMe22 = false;
		if (lhs.m_e00 <= rhs.m_e00) {
			lvMe00 = true;
		}
		if (lhs.m_e01 <= rhs.m_e01) {
			lvMe01 = true;
		}
		if (lhs.m_e02 <= rhs.m_e02) {
			lvMe02 = true;
		}
		if (lhs.m_e10 <= rhs.m_e10) {
			lvMe10 = true;
		}
		if (lhs.m_e11 <= rhs.m_e11) {
			lvMe11 = true;
		}
		if (lhs.m_e12 <= rhs.m_e12) {
			lvMe12 = true;
		}
		if (lhs.m_e20 <= rhs.m_e20) {
			lvMe20 = true;
		}
		if (lhs.m_e21 <= rhs.m_e21) {
			lvMe21 = true;
		}
		if (lhs.m_e22 <= rhs.m_e22) {
			lvMe22 = true;
		}

		return lvMe00 && lvMe01 && lvMe02 && lvMe10 && lvMe11 && lvMe12 && lvMe20 && lvMe21 && lvMe22;

	}


	bool operator>=(const Mat3x3& lhs, const Mat3x3& rhs) {
		bool lvMe00 = false, lvMe01 = false, lvMe02 = false;
		bool lvMe10 = false, lvMe11 = false, lvMe12 = false;
		bool lvMe20 = false, lvMe21 = false, lvMe22 = false;
		if (lhs.m_e00 >= rhs.m_e00) {
			lvMe00 = true;
		}
		if (lhs.m_e01 >= rhs.m_e01) {
			lvMe01 = true;
		}
		if (lhs.m_e02 >= rhs.m_e02) {
			lvMe02 = true;
		}
		if (lhs.m_e10 >= rhs.m_e10) {
			lvMe10 = true;
		}
		if (lhs.m_e11 >= rhs.m_e11) {
			lvMe11 = true;
		}
		if (lhs.m_e12 >= rhs.m_e12) {
			lvMe12 = true;
		}
		if (lhs.m_e20 >= rhs.m_e20) {
			lvMe20 = true;
		}
		if (lhs.m_e21 >= rhs.m_e21) {
			lvMe21 = true;
		}
		if (lhs.m_e22 >= rhs.m_e22) {
			lvMe22 = true;
		}

		return lvMe00 && lvMe01 && lvMe02 && lvMe10 && lvMe11 && lvMe12 && lvMe20 && lvMe21 && lvMe22;

	}
}