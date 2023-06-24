#ifndef PLANE_SHAPE_H
#define PLANE_SHAPE_H
#include "BaseShape.h"

// Planes will always be static, meaning they are non-moving, use only for demo purposes
class Plane : public BaseShape
{
public:
    int id = 0;
    static int next_id;
    Plane();
    Plane(glm::vec3 pos, glm::vec3 size, glm::quat rotation);
    void createCollisionShape() override {
        collisionShape = new btStaticPlaneShape(btVector3(btScalar(Size.x), btScalar(Size.y), btScalar(Size.z)), 0.0f);
    }
    virtual void InitModel() override;
};

#endif // !PLANE_SHAPE_H