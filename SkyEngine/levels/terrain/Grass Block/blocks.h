#ifndef BLOCKS_H
#define BLOCKS_H

#include "../../../src/CollisionShapes/BaseShape.h"
class GrassBlock : public BaseShape
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

        void InitModel() override { model = Model("levels/terrain/Grass Block/grass_block.obj"); }
        GrassBlock() : BaseShape() {}
        GrassBlock(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, velocity, rotation, colliderModel) {
            blockType = BlockType::GRASS;
            id = next_id++;
        }
        GrassBlock(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) {
            InitModel();
            blockType = BlockType::GRASS;
            id = next_id++;
        }

};
#endif // !GRASSBLOCK_H




