#include "light.h"
#include "g_shader.h"
#include <imgui.h>
int Light::next_id = 0;
int PointLight::next_id = 0;
int DirectionalLight::next_id = 0;
int SpotLight::next_id = 0;

void DirectionalLight::ShowMenu(std::string name)
{
    ImGui::SetItemDefaultFocus();
    // Bring this menu up if object is selected

    ImGui::Begin(name.c_str());

    if (ImGui::BeginChild("Child Window", ImVec2(600, 200), false))
    {
        if (ImGui::DragFloat3("Direction", (float*)&Direction)) {}

        if (ImGui::DragFloat3("Ambient", (float*)&Ambient, 0.05f, 0.0f, 1.0f)) {}

        if (ImGui::DragFloat3("Diffuse", (float*)&Diffuse, 0.05f, 0.0f, 1.0f)) {}

        if (ImGui::DragFloat3("Specular", (float*)&Specular, 0.05f, 0.0f, 1.0f)) {}
        ImGui::EndChild();
    }  ImGui::End();
}

void DirectionalLight::Update(Shader shader)
{
    shader.use();
    shader.setVec3("dirLight.direction", Direction);
    shader.setVec3("dirLight.ambient", Ambient);
    shader.setVec3("dirLight.diffuse", Diffuse);
    shader.setVec3("dirLight.specular", Specular);
}

void PointLight::ShowMenu(std::string name)
{
    ImGui::SetItemDefaultFocus();
    // Bring this menu up if object is selected

    ImGui::Begin(name.c_str());

    if (ImGui::BeginChild("Child Window", ImVec2(600, 200), false))
    {
        if (ImGui::DragFloat3("Position", (float*)&Position)) { }

        if (ImGui::DragFloat3("Ambient", (float*)&Ambient, 0.05f, 0.0f, 1.0f)) { }

        if (ImGui::DragFloat3("Diffuse", (float*)&Diffuse, 0.05f, 0.0f, 1.0f)) { }

        if (ImGui::DragFloat3("Specular", (float*)&Specular, 0.05f, 0.0f, 1.0f)) { }

        if (ImGui::DragFloat("Constant", &Constant, 0.05f, 0.0f, 1.0f)) {}

        if (ImGui::DragFloat("Linear", &Linear, 0.005f, 0.0f, 0.1f)) {}

        if (ImGui::DragFloat("Quadratic", &Quadratic, 0.005f, 0.0f, 0.1f)) {}
        ImGui::EndChild();
    }  ImGui::End();
}

void PointLight::Update(Shader shader)
{
    shader.use();
    shader.setVec3("pointLight.position", Position);
    shader.setVec3("pointLight.ambient", Ambient);
    shader.setVec3("pointLight.diffuse", Diffuse);
    shader.setVec3("pointLight.specular", Specular);
    shader.setFloat("pointLight.constant", Constant);
    shader.setFloat("pointLight.linear", Linear);
    shader.setFloat("pointLight.quadratic", Quadratic);
}

void SpotLight::ShowMenu(std::string name)
{
    ImGui::SetItemDefaultFocus();
    // Bring this menu up if object is selected

    ImGui::Begin(name.c_str());

    if (ImGui::BeginChild("Child Window", ImVec2(600, 200), false))
    {
        if (ImGui::DragFloat3("Position", (float*)&Position)) {}

        if (ImGui::DragFloat3("Direction", (float*)&Direction)) {}

        if (ImGui::DragFloat3("Ambient", (float*)&Ambient, 0.05f, 0.0f, 1.0f)) {}

        if (ImGui::DragFloat3("Diffuse", (float*)&Diffuse, 0.05f, 0.0f, 1.0f)) {}

        if (ImGui::DragFloat3("Specular", (float*)&Specular, 0.05f, 0.0f, 1.0f)) {}

        if (ImGui::DragFloat("Cut-Off", &CutOff, 1.0f, 0.0f, 90.0f)) {}

        if (ImGui::DragFloat("Outer Cut-Off", &OuterCutOff, 1.0f, 0.0f, 90.0f)) {}

        if (ImGui::DragFloat("Constant", &Constant, 0.05f, 0.0f, 1.0f)) {}

        if (ImGui::DragFloat("Linear", &Linear, 0.005f, 0.0f, 0.1f)) {}

        if (ImGui::DragFloat("Quadratic", &Quadratic, 0.005f, 0.0f, 0.1f)) {}
        ImGui::EndChild();
    }  ImGui::End();
}

void SpotLight::Update(Shader shader)
{
    // spotLight
    
    shader.use();
    shader.setVec3("spotLight.position", Position);
    shader.setVec3("spotLight.direction", Direction);
    shader.setVec3("spotLight.ambient", Ambient);
    shader.setVec3("spotLight.diffuse", Diffuse);
    shader.setVec3("spotLight.specular", Specular);
    shader.setFloat("spotLight.constant", Constant);
    shader.setFloat("spotLight.linear", Linear);
    shader.setFloat("spotLight.quadratic", Quadratic);
    shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(CutOff)));
    shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(OuterCutOff)));
}