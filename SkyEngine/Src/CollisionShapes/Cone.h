#ifndef CONE_SHAPE_H
#define CONE_SHAPE_H

#include "BaseShape.h"

class Cone : public BaseShape
{
public:
    int id = 0;
    static int next_id;

    Cone();
    Cone(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel);
    Cone(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation);
    Cone(glm::vec3 pos, glm::vec3 size, glm::quat rotation);
    Cone(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel);
    ~Cone() override;

    void InitModel() override;
    void createCollisionShape() override;

    virtual std::shared_ptr<GameObject> clone() const override;
};

#endif // !CONE_SHAPE_H