#ifndef BLOCKS_H
#define BLOCKS_H

#include "../../../g_collision.h"
class GrassBlock : public ColliderShape
{
    public:
        int id = 0;
        static int next_id;
        void createCollisionShape() override {
            collisionShape = new btBoxShape(btVector3(btScalar(Size.x), btScalar(Size.y), btScalar(Size.z)));
        }

        virtual std::shared_ptr<GameObject> clone() const override {
            return std::make_shared<GrassBlock>(*this);
        }

        GrassBlock() : ColliderShape() {}
        GrassBlock(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {
            blockType = BlockType::GRASS;
            id = next_id++;
        }

};
#endif // !GRASSBLOCK_H




