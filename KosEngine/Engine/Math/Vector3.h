/******************************************************************/
/*!
\file      Vector3.h
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      8 Nov 2024
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
#pragma warning (disable : 4201)
#pragma warning(disable : 26495)
#ifndef VECTOR3_H
#define VECTOR3_H
#include <cstddef>
#include "Mathlib.h"
//REFER TO UNITY VECTOR 3
namespace vector3 {
    class Vec3 {
    public:
        
        union {
            struct {
                float m_x, m_y, m_z;
            };
            [[maybe_unused]] float m[3];
        };

        //constructor
        Vec3();
        Vec3(float _x, float _y, float _z);

        Vec3& operator +=(const Vec3& rhs);
        Vec3& operator -=(const Vec3& rhs);
        Vec3& operator *= (float rhs);
        Vec3& operator /= (float rhs);
        Vec3  operator -() const;
        static void m_Vec3Normalize(Vec3& pResult, const Vec3& pVec0);
        static float m_Vec3Length(const Vec3& pVec0);
        static float m_Vec3SquareLength(const Vec3& pVec0);
        static float m_Vec3Distance(const Vec3& pVec0, const Vec3& pVec1);
        static float m_Vec3SquareDistance(const Vec3& pVec0, const Vec3& pVec1);
        static float m_Vec3DotProduct(const Vec3& pVec0, const Vec3& pVec1);
        static Vec3 m_Vec3CrossProduct(const Vec3& pVec0, const Vec3& pVec1);
    };
    /******************************************************************/
   /*!
   \fn        Vec3 operator + (const Vec3& lhs, const Vec3& rhs)
   \brief     Overloads the + operator to add two vectors.
   \param[in] lhs  The first vector.
   \param[in] rhs  The second vector.
   \return    A new vector resulting from the addition of the two vectors.
   */
   /******************************************************************/
    Vec3 operator + (const Vec3& lhs, const Vec3& rhs);
    /******************************************************************/
    /*!
    \fn        Vec3 operator - (const Vec3& lhs, const Vec3& rhs)
    \brief     Overloads the - operator to subtract one vector from another.
    \param[in] lhs  The first vector.
    \param[in] rhs  The second vector.
    \return    A new vector resulting from the subtraction of the two vectors.
    */
    /******************************************************************/
    Vec3 operator - (const Vec3& lhs, const Vec3& rhs);
    /******************************************************************/
    /*!
    \fn        Vec3 operator * (const Vec3& lhs, float rhs)
    \brief     Overloads the * operator to scale a vector by a scalar.
    \param[in] lhs  The vector.
    \param[in] rhs  The scalar value.
    \return    A new vector resulting from the scaling of the vector.
    */
    /******************************************************************/
    Vec3 operator * (const Vec3& lhs, float rhs);
    /******************************************************************/
    /*!
    \fn        Vec3 operator * (float lhs, const Vec3& rhs)
    \brief     Overloads the * operator to scale a vector by a scalar (commutative).
    \param[in] lhs  The scalar value.
    \param[in] rhs  The vector.
    \return    A new vector resulting from the scaling of the vector.
    */
    /******************************************************************/
    Vec3 operator * (float lhs, const Vec3& rhs);
    /******************************************************************/
    /*!
    \fn        Vec3 operator / (const Vec3& lhs, float rhs)
    \brief     Overloads the / operator to divide a vector by a scalar.
    \param[in] lhs  The vector.
    \param[in] rhs  The scalar value.
    \return    A new vector resulting from the division of the vector by the scalar.
    */
    /******************************************************************/
    Vec3 operator / (const Vec3& lhs, float rhs);
    /******************************************************************/
    /*!
    \fn        Vec3 operator * (const Vec3& lhs, const Vec3& rhs)
    \brief     Overloads the * operator to compute the dot product of two vectors.
    \param[in] lhs  The first vector.
    \param[in] rhs  The second vector.
    \return    The dot product of the two vectors.
    */
    /******************************************************************/
    Vec3 operator * (const Vec3& lhs, const Vec3& rhs); //dot product

}

#endif VECTOR3_H