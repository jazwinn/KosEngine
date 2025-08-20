/******************************************************************/
/*!
\file      Vector2.h
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

#ifndef VECTOR2_H
#define VECTOR2_H
#include <cstddef>


namespace vector2 {
    class Vec2 {
    public:
        float m_x{};
        float m_y{};

        //constructor
        Vec2() : m_x(0.0f), m_y(0.0f) {};
        Vec2(float _x, float _y);
        /******************************************************************/
        /*!
        \fn        Vec2& Vec2::operator += (const Vec2& rhs)
        \brief     Overloads the += operator to add another vector to this vector.
        \param[in] rhs  The vector to be added.
        \return    A reference to the modified vector.
        */
        /******************************************************************/
        Vec2& operator += (const Vec2& rhs);
        /******************************************************************/
        /*!
        \fn        Vec2& Vec2::operator -= (const Vec2& rhs)
        \brief     Overloads the -= operator to subtract another vector from this vector.
        \param[in] rhs  The vector to be subtracted.
        \return    A reference to the modified vector.
        */
        /******************************************************************/
        Vec2& operator -= (const Vec2& rhs);
        /******************************************************************/
        /*!
        \fn        Vec2& Vec2::operator *= (float rhs)
        \brief     Overloads the *= operator to scale this vector by a scalar value.
        \param[in] rhs  The scalar value to multiply the vector by.
        \return    A reference to the modified vector.
        */
        /******************************************************************/
        Vec2& operator *= (float rhs);
        /******************************************************************/
        /*!
        \fn        Vec2& Vec2::operator /= (float rhs)
        \brief     Overloads the /= operator to scale this vector by dividing by a scalar value.
        \param[in] rhs  The scalar value to divide the vector by.
        \return    A reference to the modified vector.
        */
        /******************************************************************/
        Vec2& operator /= (float rhs);
        /******************************************************************/
        /*!
        \fn        Vec2 Vec2::operator - () const
        \brief     Overloads the unary minus operator to negate the vector (i.e., return a vector pointing in the opposite direction).
        \return    A new vector with negated x and y values.
        */
        /******************************************************************/
        Vec2 operator -() const;
        /******************************************************************/
        /*!
        \fn        static void Vec2::m_funcVec2Normalize(Vec2& pResult, const Vec2& pVec0)
        \brief     Normalizes the given vector and stores the result in pResult.
        \param[out] pResult  The vector to store the normalized result.
        \param[in]  pVec0    The vector to be normalized.
        */
        /******************************************************************/
        static void  m_funcVec2Normalize(Vec2& pResult, const Vec2& pVec0);
        /******************************************************************/
        /*!
        \fn        static float Vec2::m_funcVec2DLength(const Vec2& pVec0)
        \brief     Computes the length (magnitude) of the given vector.
        \param[in] pVec0  The vector whose length is being calculated.
        \return    The length of the vector.
        */
        /******************************************************************/
        static float m_funcVec2DLength(const Vec2& pVec0);
        /******************************************************************/
        /*!
        \fn        static float Vec2::m_funcVec2DSquareLength(const Vec2& pVec0)
        \brief     Computes the square of the length of the given vector.
                   (Useful to avoid the square root operation when comparing lengths.)
        \param[in] pVec0  The vector whose squared length is being calculated.
        \return    The squared length of the vector.
        */
        /******************************************************************/
        static float m_funcVec2DSquareLength(const Vec2& pVec0);
        /******************************************************************/
        /*!
        \fn        static float Vec2::m_funcVec2DDistance(const Vec2& pVec0, const Vec2& pVec1)
        \brief     Computes the Euclidean distance between two vectors.
        \param[in] pVec0  The first vector.
        \param[in] pVec1  The second vector.
        \return    The distance between the two vectors.
        */
        /******************************************************************/
        static float m_funcVec2DDistance(const Vec2& pVec0, const Vec2& pVec1);

        /******************************************************************/
        /*!
        \fn        static float Vec2::m_funcVec2DSquareDistance(const Vec2& pVec0, const Vec2& pVec1)
        \brief     Computes the squared distance between two vectors.
                   (Useful to avoid the square root operation when comparing distances.)
        \param[in] pVec0  The first vector.
        \param[in] pVec1  The second vector.
        \return    The squared distance between the two vectors.
        */
        /******************************************************************/
        static float m_funcVec2DSquareDistance(const Vec2& pVec0, const Vec2& pVec1);
        /******************************************************************/
        /*!
        \fn        static float Vec2::m_funcVec2DDotProduct(const Vec2& pVec0, const Vec2& pVec1)
        \brief     Computes the dot product of two vectors.
        \param[in] pVec0  The first vector.
        \param[in] pVec1  The second vector.
        \return    The dot product of the two vectors.
        */
        /******************************************************************/
        static float m_funcVec2DDotProduct(const Vec2& pVec0, const Vec2& pVec1);
        /******************************************************************/
        /*!
        \fn        static float Vec2::m_funcVec2CrossProduct(const Vec2& pVec0, const Vec2& pVec1)
        \brief     Computes the cross product of two 2D vectors.
        \param[in] pVec0  The first vector.
        \param[in] pVec1  The second vector.
        \return    The cross product of the two vectors.
        */
        /******************************************************************/
        static float m_funcVec2CrossProduct(const Vec2& pVec0, const Vec2& pVec1);

    };
    /******************************************************************/
    /*!
    \fn        Vec2 operator + (const Vec2& lhs, const Vec2& rhs)
    \brief     Overloads the + operator to add two vectors.
    \param[in] lhs  The first vector.
    \param[in] rhs  The second vector.
    \return    A new vector resulting from the addition of the two vectors.
    */
    /******************************************************************/
    Vec2 operator + (const Vec2& lhs, const Vec2& rhs);
    /******************************************************************/
    /*!
    \fn        Vec2 operator - (const Vec2& lhs, const Vec2& rhs)
    \brief     Overloads the - operator to subtract one vector from another.
    \param[in] lhs  The first vector.
    \param[in] rhs  The second vector.
    \return    A new vector resulting from the subtraction of the two vectors.
    */
    /******************************************************************/
    Vec2 operator - (const Vec2& lhs, const Vec2& rhs);
    /******************************************************************/
    /*!
    \fn        Vec2 operator * (const Vec2& lhs, float rhs)
    \brief     Overloads the * operator to scale a vector by a scalar.
    \param[in] lhs  The vector.
    \param[in] rhs  The scalar value.
    \return    A new vector resulting from the scaling of the vector.
    */
    /******************************************************************/
    Vec2 operator * (const Vec2& lhs, float rhs);
    /******************************************************************/
    /*!
    \fn        Vec2 operator * (float lhs, const Vec2& rhs)
    \brief     Overloads the * operator to scale a vector by a scalar (commutative).
    \param[in] lhs  The scalar value.
    \param[in] rhs  The vector.
    \return    A new vector resulting from the scaling of the vector.
    */
    /******************************************************************/
    Vec2 operator * (float lhs, const Vec2& rhs);
    /******************************************************************/
    /*!
    \fn        Vec2 operator / (const Vec2& lhs, float rhs)
    \brief     Overloads the / operator to divide a vector by a scalar.
    \param[in] lhs  The vector.
    \param[in] rhs  The scalar value.
    \return    A new vector resulting from the division of the vector by the scalar.
    */
    /******************************************************************/
    Vec2 operator / (const Vec2& lhs, float rhs);
    /******************************************************************/
    /*!
    \fn        Vec2 operator * (const Vec2& lhs, const Vec2& rhs)
    \brief     Overloads the * operator to compute the dot product of two vectors.
    \param[in] lhs  The first vector.
    \param[in] rhs  The second vector.
    \return    The dot product of the two vectors.
    */
    /******************************************************************/
    Vec2 operator * (const Vec2& lhs, const Vec2& rhs); //dot product

}
#endif VECTOR2_H
