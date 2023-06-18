
#include "GameObject.h"
#include "../../../imgui/imgui.h"
#include "../../Scene/Scene.h"

// For the game objects with vs without the 'Model' argument at the end of the constructor, use the constructer with Model argument if the object model is static, 
// use the constructer without the Model argument if the object model is dynamic.

// Quaternion Constructers
GameObject::GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel) 
: Position(pos), Size(size), Velocity(velocity), hasVelocity(true) ,Rotation(rotation), 
  isEuler(false), Destroyed(false), model(objModel), modelDynamic(false)
{
    _dynamicsWorld = NULL;
    rigidBody = NULL;
    collisionShape = NULL;
    massValue = 0.0f;
    frictionValue = 0.0f;
    localInertia = btVector3(0.0f, 0.0f, 0.0f);
}

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation)
    : Position(pos), Size(size), Velocity(velocity), hasVelocity(true), Rotation(rotation), 
      isEuler(false), Destroyed(false) , model(), modelDynamic(true)
{
    _dynamicsWorld = NULL;
    rigidBody = NULL;
    collisionShape = NULL;
    massValue = 0.0f;
    frictionValue = 0.0f;
    localInertia = btVector3(0.0f, 0.0f, 0.0f);
}

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model objModel)
    : Position(pos), Size(size), Velocity(0.0f), hasVelocity(false), Rotation(rotation), 
      isEuler(false), Destroyed(false), model(objModel), modelDynamic(false)
{
    _dynamicsWorld = NULL;
    rigidBody = NULL;
    collisionShape = NULL;
    frictionValue = 0.0f;
    localInertia = btVector3(0.0f, 0.0f, 0.0f);
}

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, glm::quat rotation)
    : Position(pos), Size(size), Velocity(0.0f), hasVelocity(false), Rotation(rotation), 
      isEuler(false), Destroyed(false), model(), modelDynamic(true)
{
    _dynamicsWorld = NULL;
    rigidBody = NULL;
    collisionShape = NULL;
    frictionValue = 0.0f;
    localInertia = btVector3(0.0f, 0.0f, 0.0f);
}

// Euler Constructers
GameObject::GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::vec3 eulerRotation, Model objModel)
    : Position(pos), Size(size), Velocity(velocity), hasVelocity(true), Rotation(eulerRotation), 
      isEuler(true), Destroyed(false), model(objModel), modelDynamic(false)
{
    Rotation = glm::quat(eulerRotation);
    _dynamicsWorld = NULL;
    rigidBody = NULL;
    collisionShape = NULL;
    frictionValue = 0.0f;
    localInertia = btVector3(0.0f, 0.0f, 0.0f);
}

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::vec3 eulerRotation)
    : Position(pos), Size(size), Velocity(velocity), hasVelocity(true), Rotation(eulerRotation), 
      isEuler(true), Destroyed(false), model(), modelDynamic(true)
{
    Rotation = glm::quat(eulerRotation);
    _dynamicsWorld = NULL;
    rigidBody = NULL;
    collisionShape = NULL;
    frictionValue = 0.0f;
    localInertia = btVector3(0.0f, 0.0f, 0.0f);
}

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 eulerRotation, Model objModel)
    : Position(pos), Size(size), Velocity(0.0f), hasVelocity(false), Rotation(eulerRotation), 
      isEuler(true), Destroyed(false), model(objModel), modelDynamic(false)
{
    Rotation = glm::quat(eulerRotation);
    _dynamicsWorld = NULL;
    rigidBody = NULL;
    collisionShape = NULL;
    frictionValue = 0.0f;
    localInertia = btVector3(0.0f, 0.0f, 0.0f);
}

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 eulerRotation)
    : Position(pos), Size(size), Velocity(0.0f), hasVelocity(false), Rotation(eulerRotation), 
      isEuler(true), Destroyed(false), model(), modelDynamic(true)
{
    Rotation = glm::quat(eulerRotation);
    _dynamicsWorld = NULL;
    rigidBody = NULL;
    collisionShape = NULL;
    frictionValue = 0.0f;
    localInertia = btVector3(0.0f, 0.0f, 0.0f);
}

GameObject::GameObject(glm::vec3 pos)
    : Position(pos), Size(1.0f), Velocity(0.0f), hasVelocity(false), Rotation(glm::identity<glm::quat>()),
    isEuler(false), Destroyed(false), model(), modelDynamic(false)
{
}

GameObject::GameObject(glm::vec3 pos, glm::vec3 velocity)
    : Position(pos), Size(1.0f), Velocity(velocity), hasVelocity(false), Rotation(glm::identity<glm::quat>()),
    isEuler(false), Destroyed(false), model(), modelDynamic(false)
{
}

GameObject::GameObject() 
: Position(0.0f), Size(1.0f), Velocity(0.0f), hasVelocity(false), Rotation(glm::identity<glm::quat>()), 
  isEuler(false), Destroyed(false), model() , modelDynamic(false)
{
    _dynamicsWorld = NULL;
    rigidBody = NULL;
    collisionShape = NULL;
}

void GameObject::DrawModel(Model modelRender, Shader modelShader)
{
	modelRender.Draw(modelShader, this->Position, this->Size, this->Rotation);
}

void GameObject::DrawModel(Shader modelShader)
{
    model.Draw(modelShader, this->Position, this->Size, this->Rotation);
}

void GameObject::UpdateObject(Shader shader, btDynamicsWorld* dynamicsWorld)
{
    _dynamicsWorld = dynamicsWorld;
    DrawModel(shader);
    UpdateRigidBody();
}
void GameObject::ObjMenu(string name)
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

        if (ImGui::DragFloat3("Object Pos", (float*)&currentPosition, .5f)) {
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

        ScaleUniform("Object Size", (float*)&Size, 1.0f, 0.0f, 100.0f);

        glm::quat currentRotation = bulletToGlm(currentTransform.getRotation());

        if(ImGui::DragFloat4("Object Rot", (float*)&currentRotation, .05, -1.0f, 1.0f)) { 
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
        if (ImGui::DragFloat("Object Friction", &frictionValue)) { setFrictionValue(frictionValue); }
        if (ImGui::DragFloat("Ambient", (float*) & material.ambient, 0.05f, 0.0f, 1.0f)) { material.setAmbient(material.ambient); }
        if (ImGui::DragFloat("Diffuse", (float*) & material.diffuse, 0.05f, 0.0f, 1.0f)) { material.setDiffuse(material.diffuse); }
        if (ImGui::DragFloat("Specular", (float*) & material.specular, 0.05f, 0.0f, 1.0f)) { material.setSpecular(material.specular); }
        if (ImGui::DragFloat("Shininess", &material.shininess, 0.5f, 0.01f, 255.0f)) { material.setShininess(material.shininess); }
        
        if (ImGui::InputFloat3("Local Inertia (READ ONLY)", (float*) & localInertia, "%.3f", ImGuiInputTextFlags_ReadOnly)) { }
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

void GameObject::setMass(float newMass) {
    // Remove the rigid body from the dynamics world
    _dynamicsWorld->removeRigidBody(rigidBody);

    // Update mass value
    massValue = newMass;

    // Update isDynamic status
    isDynamic = (massValue != 0.f);

    // Update local inertia based on the new mass
    if (isDynamic) {
        collisionShape->calculateLocalInertia(massValue, localInertia);
    }
    else {
        localInertia = btVector3(0, 0, 0);
    }

    // Update the rigid body construction info
    btRigidBody::btRigidBodyConstructionInfo rbInfo(massValue, rigidBody->getMotionState(), collisionShape, localInertia);

    // Replace the existing rigid body with a new one
    delete rigidBody;
    rigidBody = new btRigidBody(rbInfo);

    // Add the new rigid body back to the dynamics world
    _dynamicsWorld->addRigidBody(rigidBody);
}

void GameObject::setFrictionValue(float newFriction)
{
    _dynamicsWorld->removeRigidBody(rigidBody);

    frictionValue = newFriction;

    // Update local inertia based on the new mass
    if (isDynamic) {
        collisionShape->calculateLocalInertia(massValue, localInertia);
    }
    else {
        localInertia = btVector3(0, 0, 0);
    }

    // Update the rigid body construction info
    btRigidBody::btRigidBodyConstructionInfo rbInfo(massValue, rigidBody->getMotionState(), collisionShape, localInertia);

    // Replace the existing rigid body with a new one
    delete rigidBody;
    rigidBody = new btRigidBody(rbInfo);

    // Add the new rigid body back to the dynamics world
    _dynamicsWorld->addRigidBody(rigidBody);

}

void GameObject::copy() {
    _copy->copyPosition = getPosition();
    _copy->copyRotation = getRotation();
    _copy->copyVelocity = getVelocity();
    _copy->copySize = getScale();
    _copy->copyMass = getMass();
    _copy->copyFriction = getFrictionValue();
    _copy->copyInertia = getLocalIntertia();

}

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
        for (size_t i = 0; i < 3; ++i)
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