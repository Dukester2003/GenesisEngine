#ifndef CYLINDER_H
#define CYLINDER_H

#include "BaseShape.h"

class Cylinder : public BaseShape
{
public:
    int id = 0;
    static int next_id;

    Cylinder();
    Cylinder(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel);
    Cylinder(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation);
    Cylinder(glm::vec3 pos, glm::vec3 size, glm::quat rotation);
    Cylinder(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel);
    ~Cylinder() override;

    void InitModel() override;
    void createCollisionShape() override;

    virtual std::shared_ptr<GameObject> clone() const override;
};

#endif // !CYLINDER_H