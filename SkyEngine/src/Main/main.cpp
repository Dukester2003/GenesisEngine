#include "../Scene/Scene.h"
#include "../Scene/glfw_setup.h"
#include <random>
#include <iostream>
#include <stb_image.h>
#include <irrklang/irrKlang.h>


#include "../math.h"

#include "../player.h"
#include "../Level.h"

#include "../Scene/Animation/Animation.h"
#include "../Scene/Animation/AnimationData.h"

#include "../CollisionShapes/BaseShape.h"
#include "../Core/Object/GameObject.h"
#include "../Main/Engine.h"
#include "../Scene/Scene.h"
#include "../Scene/Cubemap.h"
#include "../Common_Assets.h"
#include "../framebuffers.h"
#include "../InitiateCollision.h"
#include "../Scene/Grid.h"
#include <Windows.h>

using namespace irrklang;
bool pressedOnce;

void Initialize();
void Update();
void UpdateViewPort();
void processInput(GLFWwindow* window);
void GUI_SCENE(GLFWwindow* window, GUI gui);
void GUI_FRAMEBUFFER_RENDER(GLFWwindow* window);

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Model warehouseModel;
Level complex;
GUI                       gui;
CubeMap                   cubeMap;
CollisionCallback         collisionCallback;
GLFW_Setup*               glfwSetup;
InputManager*             input;

// AUDIO
ISoundEngine *SoundEngine = createIrrKlangDevice();
EngineState engineState;

void Initialize()
{
  glfwSetup = new GLFW_Setup();
  gui.ImGuiSetup(glfwSetup->window);
  input = new InputManager(glfwSetup->window);
  InitCommonShaders();
  cubeMap.BuildCubeBoxShaders();
  collisionCallback.BulletInstanceDispatch();
  scene.grid.CreateGrid();
  cubeMap.BuildCubeBox();  
  InitFrameBuffers();  
  InitMaterial(scene);
}

void Update()
{
    //UPDATE
    while (!glfwWindowShouldClose(glfwSetup->window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Bind the framebuffer
        BindFrameBuffer();        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) 
        {
            UpdateViewPort();
            input->Update();             
            collisionCallback.UpdateBtSimulation(deltaTime);
       
            // view/projection transformations
            scene.SetPerspectiveTransformations(glfwSetup->SCR_WIDTH, glfwSetup->SCR_HEIGHT);
            

            CreateShaderTransformations(scene);

            complex.DrawLevel(warehouseModel, diffuseShader);

            UpdateCommonObjects(scene);
            UpdateLight(scene);

            // Render GriD
            scene.grid.RenderGrid(gridShader, scene.view);
            if (scene.grid.gridActive && engineState == ENGINE_SCENE) { glDrawElements(GL_LINES, scene.grid.gridLength, GL_UNSIGNED_INT, NULL); }
            cubeMap.DrawSkyBox(scene);
        }
        
        
        UnbindFrameBuffer();

        // ImGUI Update
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Show the following windows
        ImGui::ShowDemoWindow();
        GUI_SCENE(glfwSetup->window, gui);
        gui.GuiInit();
        GUI_FRAMEBUFFER_RENDER(glfwSetup->window);

       
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(glfwSetup->window);
        glfwPollEvents();
    }
}

int main()
{
    Initialize();
    
    warehouseModel = Model("levels/warehouse/compound.glb"); 

    auto FloorCollider = std::make_shared<BoxCollider>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(120.0f, .1f, 120.0f), glm::vec3(0.0f), glm::vec3(0.0f));
    FloorCollider->InitiateRigidBody(dynamicsWorld, collisionShapes);
    FloorCollider->massValue = 0.0f;
    floorColliders[0] = Floor(glm::vec3(0.0f, -0.5f,0.0f), glm::vec3(120.0f, .1f, 120.0f), glm::vec3(0.0f), glm::vec3(0.0f));
    
    Update();

    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
    scene.grid.Deallocate();
    cubeMap.Deallocate();
    collisionCallback.btCleanUp();


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        scene.camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        scene.camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        scene.camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        scene.camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        scene.camera.MovementSpeed = 10.0;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        scene.camera.MovementSpeed = 2.5;


    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !pressedOnce)
    {
        if (scene.grid.gridActive) scene.grid.gridActive = false;
        else if (!scene.grid.gridActive) scene.grid.gridActive = true;
        pressedOnce = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE) { pressedOnce = false; }
}

void UpdateViewPort()
{
    glViewport(0, 0, 400, 300);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // also clear the depth buffer now!
}

void GUI_SCENE(GLFWwindow* window, GUI gui)
{
    if (ImGui::Begin("Scene", NULL, gui.sceneFlags))
    {

        // Using a Child allow to fill all the space of the window.
        // It also alows customization
        if (ImGui::BeginTabBar("Our Scene"))
        {
            if (ImGui::BeginTabItem("Scene"))
            {
                if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused()) { gui.isSceneHovered = true; ImGui::CaptureMouseFromApp(false); processInput(window); }
                else gui.isSceneHovered = false;
                // Get the size of the child (i.e. the whole draw size of the windows).
                ImVec2 wsize = ImGui::GetWindowSize();
                // Because I use the texture from OpenGL, I need to invert the V from the UV.
                ImGui::Image((void*)(intptr_t)texture, wsize, ImVec2(0, 1), ImVec2(1, 0));
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Game"))
            {
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void GUI_FRAMEBUFFER_RENDER(GLFWwindow* window)
{
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}