#ifndef COMPOUND_SHAPE_H
#define COMPOUND_SHAPE_H
#include "BaseShape.h"

// Since the compound shape is going to have multiple objects, multiple models, the models are not going to be static
class CompoundShape : public BaseShape
{
public:
    int id = 0;
    static int next_id;

    void createCollisionShape();

    // Add a child shape to the compound shape
    void AddChildShape(BaseShape* shape);

    // Remove a child shape from the compound shape
    void RemoveChildShape(BaseShape* shape);

    virtual std::shared_ptr<GameObject> clone() const override;
    void ObjMenu(string name) override;

    CompoundShape();
    CompoundShape(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation);
    CompoundShape(glm::vec3 pos, glm::vec3 size, glm::quat rotation);

private:
    std::vector<BaseShape*> childShapes;
};

#endif // !COMPOUND_SHAPE_H