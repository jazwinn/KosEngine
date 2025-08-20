#include "../Config/pch.h"
#include "Vector3.h"

namespace vector3 {
    Vec3::Vec3() {
        m_x = 0.f;
        m_y = 0.f;
        m_z = 0.f;
        m[0] = m_x;
        m[1] = m_y;
        m[2] = m_z;
    }
    Vec3::Vec3(float _x, float _y, float _z) {
        m_x = _x;
        m_y = _y;
        m_z = _z;
        m[0] = m_x;
        m[1] = m_y;
        m[2] = m_z;
    }
    Vec3& Vec3::operator +=(const Vec3 & rhs) {
        m_x = m_x + rhs.m_x;
        m_y = m_y + rhs.m_y;
        m_z = m_z + rhs.m_z;
        m[0] = m_x;
        m[1] = m_y;
        m[2] = m_z;
        return *this;
    }
    Vec3& Vec3::operator -=(const Vec3& rhs) {
        m_x = m_x - rhs.m_x;
        m_y = m_y - rhs.m_y;
        m_z = m_z - rhs.m_z;
        m[0] = m_x;
        m[1] = m_y;
        m[2] = m_z;
        return *this;
    }
    Vec3& Vec3::operator *= (float rhs) {
        m_x = m_x * rhs;
        m_y = m_y * rhs;
        m_z = m_z * rhs;
        m[0] = m_x;
        m[1] = m_y;
        m[2] = m_z;
        return *this;
    }
    Vec3& Vec3::operator /= (float rhs) {
        m_x = m_x / rhs;
        m_y = m_y / rhs;
        m_z = m_z / rhs;
        m[0] = m_x;
        m[1] = m_y;
        m[2] = m_z;
        return *this;
    }
    Vec3 Vec3::operator -() const {
        return Vec3(-m_x,-m_y,-m_z);
    }
    void Vec3::m_Vec3Normalize(Vec3& pResult, const Vec3& pVec0) {
        float distance = m_Vec3Length(pVec0);
        pResult.m_x = pVec0.m_x / distance;
        pResult.m_y = pVec0.m_y / distance;
        pResult.m_z = pVec0.m_z / distance;
    }
    float Vec3::m_Vec3Length(const Vec3& pVec0) {
        float lengthSquare = pVec0.m_x * pVec0.m_x + pVec0.m_y * pVec0.m_y + pVec0.m_z * pVec0.m_z;
        return mathlibrary::mathlib::funcSqrt(lengthSquare);
    }
    float Vec3::m_Vec3SquareLength(const Vec3& pVec0) {
        float lengthSquare = pVec0.m_x * pVec0.m_x + pVec0.m_y * pVec0.m_y + pVec0.m_z * pVec0.m_z;
        return lengthSquare;
    }
    float Vec3::m_Vec3Distance(const Vec3& pVec0, const Vec3& pVec1) {
        float tempX = pVec0.m_x - pVec1.m_x;
        float tempY = pVec0.m_y - pVec1.m_y;
        float tempZ = pVec0.m_z - pVec1.m_z;
        float lengthSquare = tempX * tempX + tempY * tempY + tempZ * tempZ;
        return mathlibrary::mathlib::funcSqrt(lengthSquare);
    }
    float Vec3::m_Vec3SquareDistance(const Vec3& pVec0, const Vec3& pVec1) {
        float tempX = pVec0.m_x - pVec1.m_x;
        float tempY = pVec0.m_y - pVec1.m_y;
        float tempZ = pVec0.m_z - pVec1.m_z;
        float lengthSquare = tempX * tempX + tempY * tempY + tempZ * tempZ;
        return lengthSquare;
    }
    float Vec3::m_Vec3DotProduct(const Vec3& pVec0, const Vec3& pVec1) {
        return pVec0.m_x * pVec1.m_x + pVec0.m_y * pVec0.m_y + pVec0.m_z * pVec1.m_z;
    }
    Vec3 Vec3::m_Vec3CrossProduct(const Vec3& pVec0, const Vec3& pVec1) {
        return Vec3(
            pVec0.m_y * pVec1.m_z - pVec0.m_z * pVec1.m_y,
            pVec0.m_z * pVec1.m_x - pVec0.m_x * pVec1.m_z,
            pVec0.m_x * pVec1.m_y - pVec0.m_y * pVec1.m_x
        );
    }
    Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
    {
        float tempX = lhs.m_x + rhs.m_x;
        float tempY = lhs.m_y + rhs.m_y;
        float tempZ = lhs.m_z + rhs.m_z;
        return Vec3(tempX, tempY, tempZ);
    }
    
    Vec3 operator - (const Vec3& lhs, const Vec3& rhs) {
        float tempX = lhs.m_x - rhs.m_x;
        float tempY = lhs.m_y - rhs.m_y;
        float tempZ = lhs.m_z - rhs.m_z;
        return Vec3(tempX, tempY, tempZ);
    }
    Vec3 operator * (const Vec3& lhs, float rhs) {
        float tempX = lhs.m_x * rhs;
        float tempY = lhs.m_y * rhs;
        float tempZ = lhs.m_z * rhs;
        return Vec3(tempX, tempY, tempZ);
    }

    Vec3 operator * (float lhs, const Vec3& rhs) {
        float tempX = lhs * rhs.m_x;
        float tempY = lhs * rhs.m_y;
        float tempZ = lhs * rhs.m_z;
        return Vec3(tempX, tempY, tempZ);
    }

    Vec3 operator / (const Vec3& lhs, float rhs) {
        float tempX = lhs.m_x / rhs;
        float tempY = lhs.m_y / rhs;
        float tempZ = lhs.m_z / rhs;
        return Vec3(tempX, tempY, tempZ);
    }

    Vec3 operator * (const Vec3& lhs, const Vec3& rhs) {
        return Vec3(
            lhs.m_y * rhs.m_z - lhs.m_z * rhs.m_y,
            lhs.m_z * rhs.m_x - lhs.m_x * rhs.m_z,
            lhs.m_x * rhs.m_y - lhs.m_y * rhs.m_x
        );
    }
}
