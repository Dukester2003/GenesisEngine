#include "light.h"
#include <imgui.h>
int Light::next_id = 0;

void Light::ShowMenu(std::string name)
{
    ImGui::SetItemDefaultFocus();
    // Bring this menu up if object is selected

    ImGui::Begin(name.c_str());

    if (ImGui::BeginChild("Child Window", ImVec2(600, 200), false))
    {
        if (ImGui::DragFloat3("Position", (float*)&Position))
        {

        }
        ImGui::EndChild();
    }  ImGui::End();
    
}