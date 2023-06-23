#ifndef SPHERE_SHAPE_H
#define SPHERE_SHAPE_H

#include "BaseShape.h"

class Sphere : public BaseShape
{
public:
    int id = 0;
    static int next_id;

    Sphere();
    Sphere(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel);
    Sphere(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation);
    Sphere(glm::vec3 pos, glm::vec3 size, glm::quat rotation);
    Sphere(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel);
    ~Sphere() override;

    void InitModel() override;
    void createCollisionShape() override;

    virtual std::shared_ptr<GameObject> clone() const override;
};

#endif // !SPHERE_SHAPE_H