#ifndef LIGHT
#define LIGHT

#include "math.h"
#include <string.h>
#include "g_model.h"
enum class LightType
{
    AMBIENT,
    DIRECTIONAL,
    SPOTLIGHT,
    POINT
};

class Light {
public:
    int id;
    static int next_id;

    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec3 Ambient;
    glm::vec3 Specular;
    glm::vec3 Diffuse;
    float ambientIntensity;
    float specularIntensity;
    LightType type;
    std::string Name;
    bool IsSelected;
    bool gizmoToggle;
    glm::vec3 gizmoSize = glm::vec3(1.0f);
    glm::quat gizmoRot = glm::identity<glm::quat>();
    Model gizmo;
    Light(glm::vec3 pos, glm::vec3 col, glm::vec3 ambient, glm::vec3 specular)
        : Position(pos), Color(col), Ambient(0.0f), Specular(0.0f)
    {
        id = next_id++;
    }
    Light(glm::vec3 pos, glm::vec3 col, float ambientIntens, float specularIntens)
        : Position(pos), Color(col), ambientIntensity(ambientIntens), specularIntensity(specularIntens)
    {
        id = next_id++;
    }

    Light(glm::vec3 pos, glm::vec3 col)
        : Position(pos), Color(col), ambientIntensity(0.1f), specularIntensity(0.1)
    {
        id = next_id++;
    }
    Light(glm::vec3 pos)
        : Position(pos), Color(1.0f), ambientIntensity(0.1f), specularIntensity(0.01)
    {
        id = next_id++;
    }
    Light()
        : Position(), Color(1.0f), ambientIntensity(0.1f), specularIntensity(0.5f)
    {
        id = next_id++;
    }

    // Getters
    glm::vec3 getPosition() { return Position; }
    glm::vec3 getColor() { return Color; }
    glm::vec3 getAmbient() { return Ambient; }
    glm::vec3 getSpecular() { return Specular; }
    glm::vec3 getDiffuse() { return Diffuse; }
    float getAmbientIntensity() { return ambientIntensity; }
    float getSpecularIntensity() { return specularIntensity; }

    // Setters
    void setPosition(glm::vec3 pos) { Position = pos; }
    void setColor(glm::vec3 col) { 
        glm::vec3 clampedColor = glm::clamp(col, 0.0f, 1.0f);
        Color = clampedColor;  
    }

    void setAmbient(glm::vec3 ambient) {
        Ambient = ambient;
    }

    void setSpecular(glm::vec3 specular) {
        Specular = specular;
    }

    void setDiffuse(glm::vec3 diffuse) {
        Diffuse = diffuse;
    }

    void setAmbientIntensity(float intensity) { 
        if (intensity < 0.0f) {
            intensity = 0.0f;
        }
        else if (intensity > 1.0f) {
            intensity = 1.0f;
        }

        ambientIntensity = intensity;
    }
    void setSpecularIntensity(float intensity) { 
        if (intensity < 0.0f) {
            intensity = 0.0f;
        }
        else if (intensity > 1.0f) {
            intensity = 1.0f;
        }

        specularIntensity = intensity;
    
    }

    void ShowMenu(std::string name);
    void InitGizmo()
    {
        gizmo = Model("eng/lightGizmo.obj");
    }
    void DrawGizmo(Shader shader)
    {
        gizmo.Draw(shader, Position, gizmoSize, gizmoRot);
    }
};

#endif // !LIGHT

