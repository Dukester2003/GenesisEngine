
#include "game_obj.h"
#include "imgui/imgui.h"

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel) 
: Position(pos), Size(size), Velocity(velocity), Rotation(rotation), Destroyed(false), model(objModel) {}
GameObject::GameObject() 
: Position(0.0f), Size(1.0f), Velocity(0.0f), Rotation(glm::identity<glm::quat>()), Destroyed(false), model() {}

void GameObject::DrawModel(Model modelRender, Shader modelShader)
{
	modelRender.Draw(modelShader, this->Position, this->Size, this->Rotation, this->model);
}

void GameObject::ObjMenu(std::string name)
{
    ImGui::SetItemDefaultFocus();
    // Bring this menu up if object is selected

    ImGui::Begin(name.c_str());

    if (ImGui::BeginChild("Child Window", ImVec2(500, 100), false))
    {
        ImGui::DragFloat3("Object Pos", (float*)&Position);
        if(ImGui::DragFloat4("Object Rot", (float*)&Rotation))
        {
            Rotation = glm::normalize(Rotation);
        }
        btScalar newMass = massValue;
        ImGui::DragFloat("Object Mass", &newMass);

        if (newMass != massValue) {
            massValue = newMass;
            // Update the rigid body with the new mass
            collisionShape->calculateLocalInertia(massValue, localInertia);
            rigidBody->setMassProps(massValue, localInertia);
            rigidBody->updateInertiaTensor();
        }
        ImGui::EndChild();
    }
    ImGui::End();  // handle selection
}