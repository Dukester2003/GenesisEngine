#ifndef BOX_SHAPE_H
#define BOX_SHAPE_H

#include "BaseShape.h"

class Box : public BaseShape
{
public:
    int id = 0;
    static int next_id;

    Box();
    Box(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel);
    Box(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation);
    Box(glm::vec3 pos, glm::vec3 size, glm::quat rotation);
    Box(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel);
    ~Box() override;

    void InitModel() override;
    void createCollisionShape() override;

    virtual std::shared_ptr<GameObject> clone() const override;
};

#endif // !BOX_SHAPE_H