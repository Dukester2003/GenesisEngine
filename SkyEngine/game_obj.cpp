
#include "game_obj.h"


GameObject::GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel) 
: Position(pos), Size(size), Velocity(velocity), Rotation(rotation), Destroyed(false), model(objModel) {}
GameObject::GameObject() 
: Position(0.0f), Size(1.0f), Velocity(0.0f), Rotation(glm::identity<glm::quat>()), Destroyed(false), model() {}

void GameObject::DrawModel(Model modelRender, Shader modelShader)
{

	modelRender.Draw(modelShader, this->Position, this->Size, this->Rotation, this->model);
}