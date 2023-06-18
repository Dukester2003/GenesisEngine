#ifndef PLANE_SHAPE_H
#define PLANE_SHAPE_H
#include "BaseShape.h"

// Planes will always be static, meaning they are non-moving, use only for demo purposes
class Plane : public BaseShape
{
public:
    int id = 0;
    static int next_id;
    void createCollisionShape() override {
        collisionShape = new btStaticPlaneShape(btVector3(btScalar(Size.x / 2), btScalar(Size.y / 2), btScalar(Size.z / 2)), 10.0f);
    }
};

#endif // !PLANE_SHAPE_H