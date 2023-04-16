#pragma once
#ifndef MATH_H
#define MATH_H
#define PI 3.141593;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <btBulletDynamicsCommon.h>

/// This header converts glm to bullet's linear math, and bullet to glm respectively.


// converts btVector3 to glm::vec3
inline glm::vec3 bulletToGlm(const btVector3& v) { return glm::vec3(v.getX(), v.getY(), v.getZ()); }

// converts glm::vec3 to btVector3  
inline btVector3 glmToBullet(const glm::vec3& v) { return btVector3(v.x, v.y, v.z); }

// converts btVector4 to glm::vec4
inline glm::vec4 bulletToGlm(const btVector4& v) { return glm::vec4(v.getX(), v.getY(), v.getZ(),v.getW()); }

// converts glm::vec4 to btVector4
inline btVector4 glmToBullet(const glm::vec4& v) { return btVector4(v.x, v.y, v.z, v.w); }

// converts glm::quat to btQuaternion
inline glm::quat bulletToGlm(const btQuaternion& q) { return glm::quat(q.getW(), q.getX(), q.getY(), q.getZ()); }

// converts btQuatermion to glm::quat
inline btQuaternion glmToBullet(const glm::quat& q) { return btQuaternion(q.x, q.y, q.z, q.w); }

// converts btMat3x3 to glm::mat3
inline btMatrix3x3 glmToBullet(const glm::mat3& m) { return btMatrix3x3(m[0][0], m[1][0], m[2][0], m[0][1], m[1][1], m[2][1], m[0][2], m[1][2], m[2][2]); }

// represents btTransform as a 4x4 matrix in glm
// btTransform does not contain a full 4x4 matrix, so this transform is lossy.
// Affine transformations are OK but perspective transformations are not.
inline btTransform glmToBullet(const glm::mat4& m)
{
    glm::mat3 m3(m);
    return btTransform(glmToBullet(m3), glmToBullet(glm::vec3(m[3][0], m[3][1], m[3][2])));
}

// 
inline glm::mat4 bulletToGlm(const btTransform& t)
{
    glm::mat4 m(NULL);
    const btMatrix3x3& basis = t.getBasis();
    // rotation
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            m[c][r] = basis[r][c];
        }
    }
    // traslation
    btVector3 origin = t.getOrigin();
    m[3][0] = origin.getX();
    m[3][1] = origin.getY();
    m[3][2] = origin.getZ();
    // unit scale
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    m[3][3] = 1.0f;
    return m;
}

// converts glm::vector3 euler angles to glm::quat
inline glm::quat eulerAnglesToQuaternion(const glm::vec3& eulerAngles) {
    return glm::quat(eulerAngles);
}

// converts glm::quat to glm::vecto3 euler angles
inline glm::vec3 quaternionToEulerAngles(const glm::quat& quaternion) {
    return glm::degrees(glm::eulerAngles(quaternion));
}

#endif // !MATH_H

