#include "../Scene/Scene.h"
#include "../Scene/glfw_setup.h"
#include <random>
#include <iostream>
#include <irrklang/irrKlang.h>


#include "../math.h"

#include "../Scene/Level.h"

#include "../Scene/Animation/Animation.h"
#include "../Scene/Animation/AnimationData.h"

#include "../CollisionShapes/BaseShape.h"
#include "../Core/Object/GameObject.h"
#include "../Main/Engine.h"
#include "../Scene/Scene.h"
#include "../Scene/Cubemap.h"
#include "../Common_Assets.h"
#include "../../src/CollisionShapes/InitiateCollision.h"
#include "../Scene/Grid.h"
#include "../Game/Water.h"
#include <Windows.h>

using namespace irrklang;
bool pressedOnce;

void Initialize();
void Update();
void processInput(GLFWwindow* window);
void ImGuiShutDown();
void DeAllocate();

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Scene*                    scene;
Framebuffer*              framebuffer;
Level*                    complex;
GUI*                      gui;
CubeMap*                  cubeMap;
CollisionCallback*        collisionCallback;
GLFW_Setup*               glfwSetup;
InputManager*             input;
// AUDIO
ISoundEngine* SoundEngine = createIrrKlangDevice();
EngineState engineState;

void Initialize()
{
  glfwSetup = new GLFW_Setup();
  scene = new Scene();
  glfwSetup->scene = scene;

  gui = new GUI();
  gui->ImGuiSetup(glfwSetup->window);

  input = new InputManager(glfwSetup->window);

  InitCommonShaders();
  framebuffer = new Framebuffer();
  framebuffer->Init();  
  InitMaterial(*scene);

  cubeMap = new CubeMap();
  cubeMap->BuildCubeBox();

  scene->grid.CreateGrid();
  collisionCallback->BulletInstanceDispatch();
  
  complex = new Level();
  complex->InitCollision(dynamicsWorld, collisionShapes);
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
        framebuffer->Bind();        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) 
        {
            scene->UpdateViewport(); // It wouldn't work if I just updated this with the 'scene' objects 'Update()'
            input->Update();             
            collisionCallback->UpdateBtSimulation(deltaTime);
       
            // view/projection transformations
            scene->SetPerspectiveTransformations(glfwSetup->SCR_WIDTH, glfwSetup->SCR_HEIGHT);
            
            CreateShaderTransformations(*scene);

            complex->DrawLevel(diffuseShader);

            scene->Update(diffuseShader, dynamicsWorld);

            // Render Grid
            scene->grid.RenderGrid(gridShader, scene->view);
            if (scene->grid.gridActive && engineState == ENGINE_SCENE) { glDrawElements(GL_LINES, scene->grid.gridLength, GL_UNSIGNED_INT, NULL); }
            cubeMap->DrawSkyBox(*scene);
        }

        framebuffer->Unbind();

        // ImGUI Update
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Show the following windows
        ImGui::ShowDemoWindow();
        gui->GuiScene(glfwSetup->window, framebuffer);
        if (gui->isSceneHovered) { processInput(glfwSetup->window); }
        gui->ShowEngineGUI(*scene);
        gui->FramebufferRender(glfwSetup->window);

       
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(glfwSetup->window);
        glfwPollEvents();
    }
}

int main()
{    
    Initialize();

    auto FloorCollider = std::make_shared<Box>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(120.0f, .1f, 120.0f), glm::vec3(0.0f), glm::vec3(0.0f));
    FloorCollider->InitiateRigidBody(dynamicsWorld, collisionShapes);
    FloorCollider->massValue = 0.0f;

    Update();

    ImGuiShutDown();

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    DeAllocate();


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    // Process movement keys
    for (const auto& [key, direction] : input->keyToAction) {
        if (input->IsKeyPressed(key)) {
            scene->camera.ProcessKeyboard(direction, deltaTime);
        }
    }

    // Process special keys
    if (input->IsKeyPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }

    if (input->IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
        scene->camera.MovementSpeed = 10.0;
    } else if (input->IsKeyReleased(GLFW_KEY_LEFT_SHIFT)) {
        scene->camera.MovementSpeed = 2.5;
    }


    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !pressedOnce)
    {
        if       (scene->grid.gridActive)   scene->grid.gridActive = false;
        else if  (!scene->grid.gridActive)  scene->grid.gridActive = true;
        pressedOnce = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE) { pressedOnce = false; }
}

void ImGuiShutDown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DeAllocate()
{
    framebuffer->Deallocate();
    scene->grid.Deallocate();
    cubeMap->Deallocate();
    collisionCallback->btCleanUp();
}