#include "Light.h"
#include "../Core/Common/Shader.h"
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

        if (ImGui::Checkbox("Blinn", &Blinn)) {}
        ImGui::EndChild();
    }  ImGui::End();
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