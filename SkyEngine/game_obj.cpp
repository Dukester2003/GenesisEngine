
#include "game_obj.h"
#include "imgui/imgui.h"
#include "scene.h"

void GameObject::ScaleUniform(const char* label, float* values, float speed = 1.0f, float min_value = 0.0f, float max_value = 0.0f)
{
    ImGui::PushID(label);
    // Display the drag float 3 as usual
    if (ImGui::DragFloat3(label, values, speed, min_value, max_value))
    {
        // Update the object's size and recreate the collision shape
        btVector3 newSize(values[0], values[1], values[2]);
        Size = bulletToGlm(newSize);
        updateSize(newSize);
    }
    

    // Add a uniform slider to modify all three values simultaneously
    ImGui::SameLine();
    ImGui::PushID("Uniform");
    float uniform = 0.0f;
    if (ImGui::SliderFloat("Uniform", &uniform, min_value, max_value))
    {
        for (int i = 0; i < 3; ++i)
        {
            // Update the object's size and recreate the collision shape
            btVector3 newSize(values[0], values[1], values[2]);
            Size = bulletToGlm(newSize);
            updateSize(newSize);
            values[i] = uniform;

        }
    }
    ImGui::PopID();

    ImGui::PopID();
}

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel) 
: Position(pos), Size(size), Velocity(velocity), Rotation(rotation), Destroyed(false), model(objModel) 
{
    _dynamicsWorld = NULL;
    rigidBody = NULL;
    collisionShape = NULL;
}
GameObject::GameObject() 
: Position(0.0f), Size(1.0f), Velocity(0.0f), Rotation(glm::identity<glm::quat>()), Destroyed(false), model() 
{
    _dynamicsWorld = NULL;
    rigidBody = NULL;
    collisionShape = NULL;
}

GameObject::~GameObject() {
    
}

void GameObject::DrawModel(Model modelRender, Shader modelShader)
{
	modelRender.Draw(modelShader, this->Position, this->Size, this->Rotation, this->model);
}

void GameObject::ObjMenu(std::string name)
{

    ImGui::SetItemDefaultFocus();
    // Bring this menu up if object is selected

    ImGui::Begin(name.c_str());

    if (ImGui::BeginChild("Child Window", ImVec2(600, 200), false))
    {
        rigidBodyEnabled = rigidBody->getActivationState() != DISABLE_SIMULATION;
        if (ImGui::Checkbox("Rigidbody Enabled?", &rigidBodyEnabled))
        {
            setRigidBodyEnabled(rigidBodyEnabled);
        }
        btScalar newMass = massValue;


        btVector3 currentPosition = rigidBody->getWorldTransform().getOrigin();
        Position = bulletToGlm(currentPosition);
        if (ImGui::DragFloat3("Object Pos", (float*)&Position)) {
            // Update the object's position
            btTransform newTransform = rigidBody->getWorldTransform();
            newTransform.setOrigin(btVector3(glmToBullet(Position)));
            rigidBody->setWorldTransform(newTransform);
        }

        ScaleUniform("Object Size", (float*)&Size, 1.0f, 0.0f, 100.0f);

        btQuaternion currentRotation = rigidBody->getWorldTransform().getRotation();
        Rotation = bulletToGlm(currentRotation);
        if(ImGui::DragFloat4("Object Rot", (float*)&Rotation, .05, -1.0f, 1.0f)) { 
            // Update the object's rotation
            Rotation = glm::normalize(Rotation);
            btTransform newTransform = rigidBody->getWorldTransform();
            newTransform.setRotation(btQuaternion(glmToBullet(Rotation)));
            rigidBody->setWorldTransform(newTransform);
        }

        ImGui::DragFloat("Object Mass", &newMass);

        if (newMass != massValue) {
            massValue = newMass;
            // Update the rigid body with the new mass
            collisionShape->calculateLocalInertia(massValue, localInertia);
            rigidBody->setMassProps(massValue, localInertia);
            rigidBody->updateInertiaTensor();
        }
        if (ImGui::Button("Delete"))
        {
            Destroyed = true;
        }
        ImGui::EndChild();
    }
    ImGui::End();  // handle selection
}

// Add this function to your Shape class to enable or disable the rigid body
void GameObject::setRigidBodyEnabled(bool enabled) {
    if (enabled) {
        // Enable the rigid body
        rigidBody->forceActivationState(ACTIVE_TAG);
        rigidBody->activate();
    }
    else {
        // Disable the rigid body
        rigidBody->forceActivationState(DISABLE_SIMULATION);
    }
}

// Add this function to your Shape class to update the collision shape and rigid body
void GameObject::updateSize(const btVector3& newSize) {
    // Remove the current rigid body from the dynamics world
    _dynamicsWorld->removeRigidBody(rigidBody);

    // Recreate the collision shape with the new size
    createCollisionShape();

    // Recalculate the local inertia if the object is dynamic
    if (isDynamic) {
        collisionShape->calculateLocalInertia(massValue, localInertia);
    }

    // Update the rigid body's mass and collision shape
    rigidBody->setMassProps(massValue, localInertia);
    rigidBody->setCollisionShape(collisionShape);

    // Add the updated rigid body back to the dynamics world
    _dynamicsWorld->addRigidBody(rigidBody);
}
