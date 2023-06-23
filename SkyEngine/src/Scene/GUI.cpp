#include "GUI.h"

bool GUI::showSaveWindow = false;
bool GUI::showLoadWindow = false;
char GUI::filename[64] = { '\0' };

void GUI::ImGuiSetup(GLFWwindow * window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}


bool GUI::IsMouseHoveringOverWindow(const char* window_name)
{
    // Get the window by name
    ImGui::SetNextWindowFocus();
    if (ImGui::Begin(window_name, nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoInputs))
    {
        ImGui::End();
        return false; // If the window is not found, the mouse is not hovering over it
    }

    // Check if the mouse is hovering over the current window
    bool is_hovered = ImGui::IsWindowHovered();

    ImGui::End();

    return is_hovered;
}

bool GUI::ListFiles(const std::string& path, std::vector<std::string>& files, std::vector<std::string>& directories) {
    DIR* dir;
    struct dirent* ent;
    struct stat sb;

    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            std::string entry_name = ent->d_name;
            std::string full_path = path + "/" + entry_name;
            if (stat(full_path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
                if (entry_name != "." && entry_name != "..") {
                    directories.push_back(entry_name);
                }
            }
            else {
                files.push_back(entry_name);
            }
        }
        closedir(dir);
        return true;
    }
    return false;
}

void GUI::FileExplorer(const std::string& title, std::string& current_path) {
    ImGui::Begin(title.c_str());

    // Change directory by clicking on the folder name
    if (ImGui::BeginCombo("Current Directory", current_path.c_str())) {
        std::vector<std::string> directories;
        ListFiles(current_path, std::vector<std::string>(), directories);
        for (const std::string& dir : directories) {
            if (ImGui::Selectable(dir.c_str())) {
                current_path += "/" + dir;
            }
        }
        ImGui::EndCombo();
    }

    // List files and directories
    ImGui::Text("Files:");
    ImGui::Separator();
    if (ImGui::Button("Go Up")) {
        if (current_path != ".") {
            std::filesystem::path parent_path = std::filesystem::path(current_path).parent_path();
            current_path = parent_path.string();
        }

    }
    ImGui::SameLine();
    if (ImGui::Button("Close")) { show_file_explorer = false; }
    ImGui::BeginChild("FileList", ImVec2(0, ImGui::GetWindowHeight() * 0.75f), true);

    std::vector<std::string> files, directories;
    if (ListFiles(current_path, files, directories)) {
        for (const std::string& dir : directories) {
            ImGui::Text("[DIR] %s", dir.c_str());
        }
        for (const std::string& file : files) {
            ImGui::Text("%s", file.c_str());
        }
    }

    ImGui::EndChild();
    ImGui::End();
}

void GUI::AddItem(std::shared_ptr<GameObject> item, btDynamicsWorld* dynamicsWorld) {
    item->massValue = 5.0f;
    item->InitiateRigidBody(dynamicsWorld, collisionShapes);
}

void GUI::ShowEngineGUI(Scene& scene)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {

            if (ImGui::MenuItem("Open..", "Ctrl+O"))
            {
                // Set a flag to show the file explorer
                show_file_explorer = true;
            }

            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                showSaveWindow = true;
            }

            if (ImGui::MenuItem("Load", "Ctrl+O"))
            {
                showLoadWindow = true;
            }

            if (ImGui::MenuItem("Close", "Ctrl+W")) { /*menuActive = false; */ }
            ImGui::EndMenu();
        }

        if (showSaveWindow) { ShowSaveWindow(scene); }
        if (showLoadWindow) { ShowLoadWindow(scene); }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Do Things */ }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* Do Things */ }
            if (ImGui::MenuItem("Cut", "Ctrl+X")) { /* Do Things */ }
            if (ImGui::MenuItem("Copy", "Ctrl+C")) { /* Do Things */ }
            if (ImGui::MenuItem("Paste", "Ctrl+V")) { /* Do Things */ }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options"))
        {
            if (ImGui::MenuItem("Quality")) { /*Do Things */ }
            if (ImGui::MenuItem("Toggle Grid", "Ctrl+G")) {}
            if (ImGui::MenuItem("Scene Options"))
            {

            }
            if (ImGui::MenuItem("Enable Gizmos", NULL, &scene.enableGizmos)) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Add"))
        {
            if (ImGui::BeginMenu("Shape"))
            {
                if (ImGui::MenuItem("Box"))
                {
                    auto boxCollider = std::make_shared<Box>(glm::vec3(spawnPosition), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f));
                    AddItem(boxCollider, dynamicsWorld);
                    scene.items.push_back(boxCollider);
                }

                if (ImGui::MenuItem("Sphere"))
                {
                    auto sphereCollider = std::make_shared<Sphere>(glm::vec3(spawnPosition), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f));
                    AddItem(sphereCollider, dynamicsWorld);
                    scene.items.push_back(sphereCollider);
                }

                if (ImGui::MenuItem("Cylinder"))
                {
                    auto cylinderCollider = std::make_shared<Cylinder>(glm::vec3(spawnPosition), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f));
                    AddItem(cylinderCollider, dynamicsWorld);
                    scene.items.push_back(cylinderCollider);
                }

                if (ImGui::MenuItem("Capsule"))
                {
                    auto capsuleCollider = std::make_shared<Capsule>(glm::vec3(spawnPosition), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f));
                    AddItem(capsuleCollider, dynamicsWorld);
                    scene.items.push_back(capsuleCollider);
                }

                if (ImGui::MenuItem("Cone"))
                {
                    auto coneCollider = std::make_shared<Cone>(glm::vec3(spawnPosition), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f));
                    AddItem(coneCollider, dynamicsWorld);
                    scene.items.push_back(coneCollider);
                }

                if (ImGui::MenuItem("Compound Shape"))
                {
                    auto compoundShape = std::make_shared<CompoundShape>(glm::vec3(spawnPosition), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f));
                    AddItem(compoundShape, dynamicsWorld);
                    scene.items.push_back(compoundShape);
                }
                if (ImGui::MenuItem("Monke"))
                {

                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Light Source"))
            {
                if (ImGui::MenuItem("Point Light"))
                {
                    scene.pointLights.push_back(PointLight(glm::vec3(spawnPosition)));
                    scene.pointLightPresent = true;
                }

                if (ImGui::MenuItem("Spot Light"))
                {
                    scene.spotLights.push_back(SpotLight(glm::vec3(spawnPosition), glm::vec3(0.0f)));
                    scene.spotLightPresent = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (show_file_explorer) {
        FileExplorer("File Explorer", current_path);
    }

    ImGui::Begin("Assets", NULL, flags);
    ////////////////////////////////////////////////////////
    ///                   SPAWN MENU                     ///
    ////////////////////////////////////////////////////////

    if (ImGui::CollapsingHeader("Objects"))
    {
        if (ImGui::TreeNode("Platforms"))
        {
            if (ImGui::TreeNode("sb"))
            {
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    }
    if (ImGui::CollapsingHeader("Entities"))
    {

    }
    if (ImGui::CollapsingHeader("Level Elements"))
    {
        if (ImGui::TreeNode("Terrain"))
        {
            if (ImGui::Button("Grass Block"))
            {
                auto grassBlock = std::make_shared<GrassBlock>(glm::vec3(spawnPosition), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f));
                AddItem(grassBlock, dynamicsWorld);
                scene.items.push_back(grassBlock);
            }
            ImGui::TreePop();
        }

    }
    if (ImGui::CollapsingHeader("Misc"))
    {

    }
    ImGui::End();


    if (ImGui::Begin("Objects In Scene", NULL, flags))
    {
        if (ImGui::Button("Play"))
        {

        }
        ImGui::SameLine();
        if (ImGui::Button("Stop"))
        {

        }
        ImGui::SameLine();
        if (ImGui::Button("Pause"))
        {

        }
        if (ImGui::Button("Paste"))
        {
            if (copiedObject) {
                scene.items.push_back(copiedObject->clone());
            }
        }

        /////////////////////////////////////
        ///             OBJECTS           ///
        /////////////////////////////////////
        if (ImGui::BeginListBox("Objects in Scene:", ImVec2(250, 1500)))
        {
            // Add counters for each shape type
            int boxCounter = 0;
            int sphereCounter = 0;
            int cylinderCounter = 0;
            int capsuleCounter = 0;
            int coneCounter = 0;

            static int selectedItem = 0;
            scene.itemIndex = 0;
            for (auto& item : scene.items)
            {
                switch (item->type) {
                case ShapeType::PLANE:
                    item->Name = "Plane " + std::to_string(static_cast<Plane*>(item.get())->id);
                    break;
                case ShapeType::BOX:
                    item->Name = "Box " + std::to_string(static_cast<Box*>(item.get())->id);
                    break;
                case ShapeType::SPHERE:
                    item->Name = "Sphere " + std::to_string(static_cast<Sphere*>(item.get())->id);
                    break;
                case ShapeType::CYLINDER:
                    item->Name = "Cylinder " + std::to_string(static_cast<Cylinder*>(item.get())->id);
                    break;
                case ShapeType::CAPSULE:
                    item->Name = "Capsule " + std::to_string(static_cast<Capsule*>(item.get())->id);
                    break;
                case ShapeType::CONE:
                    item->Name = "Cone " + std::to_string(static_cast<Cone*>(item.get())->id);
                    break;
                case ShapeType::COMPOUND:
                    item->Name = "Compound Shape " + std::to_string(static_cast<CompoundShape*>(item.get())->id);
                    break;
                case ShapeType::CONVEXHULL:
                    item->Name = "Convex Shape " + std::to_string(static_cast<ConvexHull*>(item.get())->id);
                    break;
                case ShapeType::TRIANGLEMESH:
                    item->Name = "Triangle Mesh " + std::to_string(static_cast<TriangleMesh*>(item.get())->id);
                    break;
                case ShapeType::HEIGHTFIELD:
                    item->Name = "HeightField " + std::to_string(static_cast<HeightField*>(item.get())->id);
                default:
                    item->Name = "Unkown";
                    break;
                    // Add more cases for other shape types if needed
                }

                switch (item->blockType) {
                case BlockType::GRASS:
                    item->Name = "GrassBlock " + std::to_string(static_cast<GrassBlock*>(item.get())->id);
                }
                item->IsSelected = (selectedItem == scene.itemIndex);
                if (ImGui::Selectable(item->Name.c_str(), item->IsSelected)) {
                    selectedItem = scene.itemIndex;
                }
                if (item->IsSelected) {
                    // Call the item menu function for the selected object
                    item->ObjMenu(item->Name);
                }



                scene.itemIndex++;
                // ... Use the 'name' string to display the object in ImGui ...


            }
            for (auto it = scene.items.begin(); it != scene.items.end(); /* no increment here */) {
                if ((*it)->Destroyed) {
                    it = scene.items.erase(it);  // erase returns the new iterator position
                }
                else {
                    ++it;
                }
            }
            if (ImGui::Button("Copy"))
            {
                if (selectedItem >= 0 && selectedItem < scene.items.size()) {
                    copiedObject = scene.items[selectedItem]->clone();
                }
            }
            ImGui::EndListBox();
            // After the ImGui::EndListBox() line

        }
        ImGui::End();
    }


    ///////////////////////////////////
    ///           LIGHTS            ///
    ///////////////////////////////////
    if (ImGui::Begin("Light Sources In Scene"), NULL, flags)
    {
        if (ImGui::BeginListBox("Light Scene In Scene", ImVec2(250, 1500)))
        {
            size_t lightIndex = 0;
            static int selectedItem = 0;

            {
                auto& dirLight = scene.dirLight;

                dirLight->Name = "DirectionalLight";
                dirLight->IsSelected = (selectedItem == lightIndex);

                if (ImGui::Selectable(dirLight->Name.c_str(), dirLight->IsSelected)) {
                    selectedItem = lightIndex;
                }

                if (dirLight->IsSelected) {
                    dirLight->ShowMenu(dirLight->Name);
                }

                lightIndex++;
            }
            for (int i = 0; i < scene.pointLights.size(); i++)
            {
                auto& pointLight = scene.pointLights[i];

                pointLight.Name = "PointLight ";
                std::string name = pointLight.Name + std::to_string(i);
                pointLight.IsSelected = (selectedItem == lightIndex);
                if (ImGui::Selectable(name.c_str(), pointLight.IsSelected)) {
                    selectedItem = lightIndex;

                }
                if (pointLight.IsSelected) {
                    // Call the item menu function for the selected object
                    pointLight.ShowMenu(pointLight.Name);
                }
                lightIndex++;
            }

            for (int i = 0; i < scene.spotLights.size(); i++)
            {
                auto& spotLight = scene.spotLights[i];

                spotLight.Name = "SpotLight ";
                std::string name = spotLight.Name + std::to_string(i);
                spotLight.IsSelected = (selectedItem == lightIndex);
                if (ImGui::Selectable(name.c_str(), spotLight.IsSelected)) {
                    selectedItem = lightIndex;
                }
                if (spotLight.IsSelected) {
                    // Call the item menu function for the selected object
                    spotLight.ShowMenu(spotLight.Name);
                }
                lightIndex++;
            }



            ImGui::EndListBox();
        }
        ImGui::End();
    }


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::ShowSaveWindow(Scene& scene) {
    if (ImGui::Begin("Name File"))  // The window will close when showSaveWindow is set to false
    {
        if (ImGui::InputText("Filename", filename, IM_ARRAYSIZE(filename), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            // This block is executed when the text in the input box changes.
            // Check if filename is not empty before saving the scene.
            if (strlen(filename) > 0)
            {
                scene.SaveScene(filename, scene.items);
                showSaveWindow = false; // Close the dialog when the scene is saved.
            }
            else
            {
                // Display an error message or handle the error appropriately
                std::cerr << "Error: Filename cannot be empty.\n";
            }
        }

        if (ImGui::Button("Cancel")) {
            showSaveWindow = false; // Close the dialog without saving
        }
    } ImGui::End();
}

void GUI::ShowLoadWindow(Scene& scene)
{
    if (ImGui::Begin("Load File"))
    {
        if (ImGui::InputText("Filenam", filename, IM_ARRAYSIZE(filename), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            // This block is executed when the text in the input box changes.
            // Check if filename is not empty before saving the scene.
            if (strlen(filename) > 0)
            {
                scene.SaveScene(filename, scene.items);
                showLoadWindow = false; // Close the dialog when the scene is saved.
            }
            else
            {
                // Display an error message or handle the error appropriately
                std::cerr << "Error: Filename cannot be empty.\n";
            }
        }

        if (ImGui::Button("Cancel")) {
            showLoadWindow = false; // Close the dialog without saving
        }
    } ImGui::End();
}