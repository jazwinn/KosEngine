/******************************************************************/
/*!
\file      vector2.cpp
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      Sept 28, 2024
\brief     This file contains the definition and implementation
           of 2D vector (Vec2) operations such as addition,
           subtraction, normalization, dot product, and cross product.

The Vec2 class provides methods for handling basic 2D vector math
operations, which are useful in many mathematical and graphical computations.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
#include "Vector2.h"


vector2::Vec2::Vec2(float _x, float _y) {
    m_x = _x;
    m_y = _y;
}


vector2::Vec2& vector2::Vec2::operator += (const Vec2& rhs) {
    m_x = m_x + rhs.m_x;
    m_y = m_y + rhs.m_y;
    return *this;
}

vector2::Vec2& vector2::Vec2::operator -= (const Vec2& rhs) {
    m_x = m_x - rhs.m_x;
    m_y = m_y - rhs.m_y;
    return *this;
}

vector2::Vec2& vector2::Vec2::operator *= (float rhs) {
    m_x = m_x * rhs;
    m_y = m_y * rhs;
    return *this;
}

vector2::Vec2& vector2::Vec2::operator /= (float rhs) {
    m_x = m_x / rhs;
    m_y = m_y / rhs;
    return *this;
}

vector2::Vec2 vector2::Vec2::operator - () const {
    return Vec2(-m_x, -m_y);
}

vector2::Vec2 vector2::operator + (const vector2::Vec2& lhs, const vector2::Vec2& rhs) {
    float tempX = lhs.m_x + rhs.m_x;
    float tempY = lhs.m_y + rhs.m_y;
    return vector2::Vec2(tempX, tempY);
}

vector2::Vec2 vector2::operator - (const vector2::Vec2& lhs, const vector2::Vec2& rhs) {
    float tempX = lhs.m_x - rhs.m_x;
    float tempY = lhs.m_y - rhs.m_y;
    return vector2::Vec2(tempX, tempY);
}

vector2::Vec2 vector2::operator * (const vector2::Vec2& lhs, float rhs) {
    float tempX = lhs.m_x * rhs;
    float tempY = lhs.m_y * rhs;
    return vector2::Vec2(tempX, tempY);
}

vector2::Vec2 vector2::operator * (float lhs, const vector2::Vec2& rhs) {
    float tempX = rhs.m_x * lhs;
    float tempY = rhs.m_y * lhs;
    return vector2::Vec2(tempX, tempY);
}

vector2::Vec2 vector2::operator * (const Vec2& lhs, const Vec2& rhs) {
    float tempX = lhs.m_x * rhs.m_x;
    float tempY = lhs.m_y * rhs.m_y;
    return vector2::Vec2(tempX, tempY);
}

vector2::Vec2 vector2::operator / (const vector2::Vec2& lhs, float rhs) {
    float tempX = lhs.m_x / rhs;
    float tempY = lhs.m_y / rhs;
    return vector2::Vec2(tempX, tempY);
}


void vector2::Vec2::m_funcVec2Normalize(vector2::Vec2& pResult, const vector2::Vec2& pVec0) {
    vector2::Vec2 temp = pVec0;
    float distance = vector2::Vec2::m_funcVec2DLength(temp);
    pResult.m_x = temp.m_x / distance;
    pResult.m_y = temp.m_y / distance;
}

float  vector2::Vec2::m_funcVec2DLength(const Vec2& pVec0) {
    float tempX = pVec0.m_x;
    float tempY = pVec0.m_y;

    float lengthSquare = tempX * tempX + tempY * tempY;
    return mathlibrary::mathlib::funcSqrt(lengthSquare);

}

float  vector2::Vec2::m_funcVec2DSquareLength(const Vec2& pVec0) {
    float tempX = pVec0.m_x;
    float tempY = pVec0.m_y;
    float lengthSquare = tempX * tempX + tempY * tempY;
    return lengthSquare;
}

float  vector2::Vec2::m_funcVec2DDistance(const Vec2& pVec0, const Vec2& pVec1) {
    float tempDiffX = pVec0.m_x - pVec1.m_x;
    float tempDiffY = pVec0.m_x - pVec1.m_y;
    float lengthSquare = tempDiffX * tempDiffX + tempDiffY * tempDiffY;
    return mathlibrary::mathlib::funcSqrt(lengthSquare);
}

float  vector2::Vec2::m_funcVec2DSquareDistance(const Vec2& pVec0, const Vec2& pVec1) {
    float tempDiffX = pVec0.m_x - pVec1.m_x;
    float tempDiffY = pVec0.m_y - pVec1.m_y;
    float lengthSquare = tempDiffX * tempDiffX + tempDiffY * tempDiffY;
    return lengthSquare;
}

float  vector2::Vec2::m_funcVec2DDotProduct(const Vec2& pVec0, const Vec2& pVec1) {
    float tempX = pVec0.m_x * pVec1.m_x;
    float tempY = pVec0.m_y * pVec1.m_y;
    float result = tempX + tempY;
    return result;
}

float vector2::Vec2::m_funcVec2CrossProduct(const Vec2& pVec0, const Vec2& pVec1) {
    float temp1 = pVec0.m_x * pVec1.m_y;
    float temp2 = pVec0.m_y * pVec1.m_x;
    float result = temp1 - temp2;
    return result;
}