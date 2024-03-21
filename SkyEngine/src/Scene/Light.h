#ifndef LIGHT
#define LIGHT

#include "math.h"
#include <string.h>
#include "../Core/Common/Model.h"
enum class LightType
{
    DIRECTIONAL,
    SPOTLIGHT,
    POINT
};

class Light {
public:
    int id;
    static int next_id;

    glm::vec3 Ambient;
    glm::vec3 Specular;
    glm::vec3 Diffuse;
    glm::vec3 Influence;
    LightType type;
    std::string Name;
    bool Blinn;
    bool IsSelected;
    bool gizmoToggle;
    glm::vec3 gizmoSize = glm::vec3(1.0f);
    glm::quat gizmoRot = glm::identity<glm::quat>();
    Model gizmo;
    Light()
        : Ambient(0.3f), Diffuse(0.3f), Specular(0.3f)
    {
        id = next_id++;
    }

    // Getters
    glm::vec3 getAmbient() { return Ambient; }
    glm::vec3 getSpecular() { return Specular; }
    glm::vec3 getDiffuse() { return Diffuse; }

    // Setters
    void setAmbient(glm::vec3 ambient) { Ambient = ambient; }
    void setSpecular(glm::vec3 specular) { Specular = specular; }
    void setDiffuse(glm::vec3 diffuse) { Diffuse = diffuse; }

    virtual void ShowMenu(std::string name) = 0;    
};

class PointLight : public Light
{
public:
    int id;
    static int next_id;

    glm::vec3 Position;
    float Constant;
    float Linear;
    float Quadratic;
    PointLight(glm::vec3 pos) : Light(), Position(pos), Constant(0.5), Linear(0.05), Quadratic(0.00) {
        type = LightType::POINT;
        id = next_id;
    }

    PointLight() {}

    bool modelDrawn = false;
    void InitGizmo()
    {
        gizmo = Model("eng/lightGizmo.obj");
    }
    void DrawGizmo(Shader shader)
    {
        gizmo.Draw(shader, Position, glm::vec3(1.0f), glm::vec3(0.0f));
    }

    // Getters
    glm::vec3 getPosition() { return Position; }
    float getConstant() { return Constant; }
    float getLinear() { return Linear; }
    float getQuadratic() { return Quadratic; }

    // Setter
    void setPosition(const glm::vec3 newPosition) { Position = newPosition; }
    virtual void ShowMenu(std::string name) override;
};

class DirectionalLight : public Light 
{
public:
    int id;
    static int next_id;
    glm::vec3 Direction;
    DirectionalLight(glm::vec3 direction) : Light(), Direction(direction) {
        type = LightType::DIRECTIONAL;
        id = next_id;
    }
    DirectionalLight() {}

    virtual void ShowMenu(std::string name) override;

    glm::vec3 getDirection() { return Direction; }
    void setDirection(const glm::vec3 newDirection) { Direction = newDirection; }
};

class SpotLight : public Light
{
public:
    int id;
    static int next_id;

    glm::vec3 Position;
    glm::vec3 Direction;
    float CutOff;
    float OuterCutOff;

    float Constant;
    float Linear;
    float Quadratic;
    SpotLight(glm::vec3 pos, glm::vec3 direction) : Light(), Position(pos), 
    Direction(direction), CutOff(0.3f), OuterCutOff(0.3f),
    Constant(0.43f), Linear(0.089f), Quadratic(0.056f)
    {
        type = LightType::SPOTLIGHT;
        id = next_id;
    }

    SpotLight() {}

    // Getters
    glm::vec3 getPosition() { return Position; }
    float getConstant() { return Constant; }
    float getLinear() { return Linear; }
    float getQuadratic() { return Quadratic; }
    float getCutOff() { return CutOff; }
    float getOuterCutOff() { return OuterCutOff; }
    // Setter
    void setPosition(const glm::vec3 newPosition) { Position = newPosition; }

    virtual void ShowMenu(std::string name) override;
};

#endif // !LIGHT
