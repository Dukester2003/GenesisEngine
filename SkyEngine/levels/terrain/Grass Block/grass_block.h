#ifndef GRASSBLOCK_H
#define GRASSBLOCK_H

#include "../../../g_collision.h"
class GrassBlock : public ColliderShape
{
    public:
        void createCollisionShape() override {
            collisionShape = new btBoxShape(btVector3(btScalar(Size.x), btScalar(Size.y), btScalar(Size.z)));
        }
        GrassBlock() : ColliderShape() {}
        GrassBlock(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {}
};
#endif // !GRASSBLOCK_H




