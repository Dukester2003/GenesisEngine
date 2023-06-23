#include "CompoundShape.h"

int CompoundShape::next_id = 0;

void CompoundShape::createCollisionShape() {
    btCompoundShape* compound = new btCompoundShape();

    for (BaseShape* childShape : childShapes) {
        childShape->createCollisionShape();
        btTransform childTransform; // set this as necessary
        compound->addChildShape(childTransform, childShape->getBtCollisionShape());
    }

    // Store 'compound' somewhere safe where it can be deleted later
    setBtCollisionShape(compound);
}

// Add a child shape to the compound shape
void CompoundShape::AddChildShape(BaseShape* shape) {
    childShapes.push_back(shape);
}

// Remove a child shape from the compound shape
void CompoundShape::RemoveChildShape(BaseShape* shape) {
    //... implement this
}

std::shared_ptr<GameObject> CompoundShape::clone() const {
    return std::make_shared<CompoundShape>(*this);
}

CompoundShape::CompoundShape() : BaseShape() {}
CompoundShape::CompoundShape(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) {
    type = ShapeType::COMPOUND;
    id = next_id++;
}
CompoundShape::CompoundShape(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : BaseShape(pos, size, rotation) {
    type = ShapeType::COMPOUND;
    id = next_id++;
}


void CompoundShape::ObjMenu(string name)
{
    ImGui::SetItemDefaultFocus();
    // Bring this menu up if object is selected

    ImGui::Begin(name.c_str());

    if (ImGui::BeginChild("Child Window", ImVec2(600, 200), false))
    {
        if (rigidBody) { rigidBodyEnabled = rigidBody->getActivationState() != DISABLE_SIMULATION; }
        if (ImGui::Checkbox("Rigidbody Enabled?", &rigidBodyEnabled))
        {
            setRigidBodyEnabled(rigidBodyEnabled);
        }
        btScalar newMass = massValue;


        btTransform currentTransform;

        glm::vec3 currentPosition;

        if (rigidBody != nullptr) {
            currentTransform = rigidBody->getWorldTransform();
            currentPosition = bulletToGlm(currentTransform.getOrigin());
        }

        if (ImGui::DragFloat3("Compound Pos", (float*)&currentPosition, .5f)) {
            // Update the object's position
            if (isDynamic && rigidBodyEnabled)
            {
                rigidBody->setActivationState(DISABLE_SIMULATION);
                btTransform newTransform = currentTransform;
                newTransform.setOrigin(glmToBullet(currentPosition));
                rigidBody->setWorldTransform(newTransform);
                setRigidBodyEnabled(rigidBodyEnabled);
            }
            else if (!isDynamic)
            {
                btTransform newTransform = currentTransform;
                newTransform.setOrigin(glmToBullet(currentPosition));
                rigidBody->setWorldTransform(newTransform);
            }
        }

        ScaleUniform("Global Size", (float*)&Size, 1.0f, 0.0f, 100.0f);

        glm::quat currentRotation = bulletToGlm(currentTransform.getRotation());

        if (ImGui::DragFloat4("Global Rot", (float*)&currentRotation, .05, -1.0f, 1.0f)) {
            // Update the object's rotation
            if (isDynamic && rigidBodyEnabled)
            {
                rigidBody->setActivationState(DISABLE_SIMULATION);
                currentRotation = glm::normalize(currentRotation);
                btTransform newTransform = rigidBody->getWorldTransform();
                newTransform.setRotation(glmToBullet(currentRotation));
                rigidBody->setWorldTransform(newTransform);
                setRigidBodyEnabled(rigidBodyEnabled);
            }
            else if (!isDynamic)
            {
                btTransform newTransform = currentTransform;
                newTransform.setRotation(glmToBullet(currentRotation));
                rigidBody->setWorldTransform(newTransform);
            }
        }

        if (ImGui::DragFloat("Object Mass", &massValue)) { setMass(massValue); }

        if (ImGui::Button("Delete"))
        {
            Destroyed = true;
        }

        ImGui::EndChild();
    }
    ImGui::End();  // handle selection
}