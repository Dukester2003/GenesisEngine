#ifndef CAPSULE_SHAPE_H
#define CAPSULE_SHAPE_H

#include "BaseShape.h"

class Capsule : public BaseShape
{
public: 
    int id = 0;
    static int next_id;

    Capsule();
    Capsule(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel);
    Capsule(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation);
    Capsule(glm::vec3 pos, glm::vec3 size, glm::quat rotation);
    Capsule(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel);
    ~Capsule() override;
 
    void InitModel() override;
    void createCollisionShape() override;

    virtual std::shared_ptr<GameObject> clone() const override;
};

#endif // !CAPSULE_SHAPE_H