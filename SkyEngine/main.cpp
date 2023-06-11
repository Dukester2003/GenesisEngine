#include "GUI.h"
#include "glfw_setup.h"
#include <random>
#include <iostream>
#include <stb_image.h>
#include <irrklang/irrKlang.h>


#include "math.h"

#include "player.h"
#include "Level.h"

#include "Shader.h"
#include "g_animator.h"
#include "animFlag.h"

#include "ShapesGeneral.h"
#include "GameObject.h"
#include "enemy.h"
#include "Engine.h"
#include "Scene.h"
#include "Cubemap.h"
#include "Common_Assets.h"
#include "framebuffers.h"
#include "InitiateCollision.h"
#include "Grid.h"
#include <Windows.h>

using namespace irrklang;
bool jumpKeyHeld;
bool isJumping;


// Jump Code and Animation Code
int numOfJumps;
bool pressedOnce;
bool spacePressed;
bool didIterateToNextJump;
bool didLastJump;
bool jumpSoundPlayed;
std::mt19937 mt(std::random_device{}());
std::uniform_int_distribution<int> jumpRand(0, 1); // generates random integers between 1 and 2
int jump_random_integer;
std::string jump[] = {"audio/Huah!.wav" , "audio/Hoo!.wav"};

// AUDIO
ISoundEngine *SoundEngine = createIrrKlangDevice();
EngineState engineState;



void processInput(GLFWwindow* window);
void UpdateCrouchActions(GLFWwindow* window);
void GUI_SCENE(GLFWwindow* window);
void GUI_FRAMEBUFFER_RENDER(GLFWwindow* window);
//bool on_mouse_click_collision(GLFWwindow* window, float xpos, float ypos, GameObject gameObject);

// Math
glm::vec3 gravity = glm::vec3(0.0f, -0.1f, 0.0f);


// grid slices
Grid grid;




// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;




// The Levels
Level lvl_1;



GUI gui;



CubeMap cubeMap;

int main()
{
    InitiateGLFW();


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Engine", NULL, NULL);
    GLFWSetup(window);
    
    

    

    
   

    // Level Models
    // ------------
    Model lvl_1_model("levels/warehouse/compound.glb");

    // Level Element Models/Animation
    // ------------

    CollisionCallback collisionCallback;
    InitCommonShaders();
    cubeMap.BuildCubeBoxShaders();
    collisionCallback.BulletInstanceDispatch();

    player = new Player(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f));
    ///////////////////////////////////////////////////////////
    ///                   FLOOR COLLIDERS                   ///
    ///////////////////////////////////////////////////////////
    auto FloorCollider = std::make_shared<BoxCollider>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(120.0f, .1f, 120.0f), glm::vec3(0.0f), glm::vec3(0.0f));
    FloorCollider->InitiateRigidBody(dynamicsWorld);
    FloorCollider->massValue = 0.0f;

    floorColliders[0] = Floor(glm::vec3(0.0f, -0.5f,0.0f), glm::vec3(120.0f, .1f, 120.0f), glm::vec3(0.0f), glm::vec3(0.0f));
    grid.CreateGrid();
    cubeMap.BuildCubeBox();  
    gui.ImGuiSetup(window);
    InitFrameBuffers();  
    InitMaterial(scene);
    player->InitAnimations();
    //UPDATE
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Bind the framebuffer
        BindFrameBuffer();
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) 
        {
            glViewport(0, 0, 400, 300);
            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // also clear the depth buffer now!

            player->UpdatePlayer(deltaTime);
            DoCollisions();
                 

            if (!player->isGrounded)
            {
                player->Position.y += gravity.y;
            }
        
        
            // per-frame time logic	
            // --------------------	
        
        
            
            player->animator.UpdateAnimation(deltaTime);
            collisionCallback.UpdateBtSimulation(deltaTime);

            


            // view/projection transformations
            projection = glm::perspective(glm::radians(scene.camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
            view = scene.camera.GetViewMatrix();

            player->BoneTransforms(animationShader);
      

            CreateShaderTransformations();

            /// LEVELS
            /// ------
            lvl_1.DrawLevel(lvl_1_model, diffuseShader);
            diffuseShader.setFloat("material.shininess", lvl_1.material.shininess);
            diffuseShader.setVec3("material.ambient", lvl_1.material.ambient);
            diffuseShader.setVec3("material.diffuse", lvl_1.material.diffuse);
            diffuseShader.setVec3("material.specular", lvl_1.material.specular); // specular lighting doesn't have full effect on this object's material
            

            UpdateCommonObjects(scene);
            UpdateLight(scene);

            // Render Grid
            {
                // Grid Shader
                grid.RenderGrid();

                // render grid
                if (grid.gridActive && engineState == ENGINE_SCENE)
                {
                    glDrawElements(GL_LINES, grid.gridLength, GL_UNSIGNED_INT, NULL);
                }
            }

            {
                cubeMap.DrawSkyBox(scene);
            }
            
        }
        
       
        UnbindFrameBuffer();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::ShowDemoWindow();
        
        

        GUI_SCENE(window);
        gui.GuiInit();
        GUI_FRAMEBUFFER_RENDER(window);

        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
    grid.Deallocate();
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
    numOfJumps = glm::clamp(numOfJumps, 0, 3);

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
        if (grid.gridActive) grid.gridActive = false;
        else if (!grid.gridActive) grid.gridActive = true;
        pressedOnce = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE) { pressedOnce = false; }

    

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !player->isCrouched)
    {
        player->MoveForward(deltaTime);
        player->isMoving = true;
        if (player->isGrounded)
        {
            player->animID = PLAYER_RUN_ID;
        }
        
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) 
    { 
        player->isMoving = false;
        if (player->isGrounded)
        {
            player->animID = PLAYER_IDLE_ID;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && !player->isCrouched && player->isGrounded)
    {
        
        glm::quat currentRotation = player->Rotation;
        glm::quat deltaRotation = glm::angleAxis(glm::radians(-2.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        player->Rotation = currentRotation * deltaRotation;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && !player->isCrouched && player->isGrounded)
    {
        glm::quat currentRotation = player->Rotation;
        glm::quat deltaRotation = glm::angleAxis(glm::radians(2.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        player->Rotation = currentRotation * deltaRotation;
    }
    
    // Crouch
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        player->crouchTransTime += deltaTime;
        player->isCrouched = true;
        if (player->isGrounded && player->crouchTransTime <= player->animations.crouchTransition.GetDuration())
        {
            player->animID = PLAYER_CROUCH_TRANSITION_ID;       
            player->transitionedToCrouch = true;
        }
        if (player->isGrounded && player->transitionedToCrouch && player->crouchTransTime >= player->animations.crouchTransition.GetDuration())
        {
            player->animID = PLAYER_CROUCH_ID;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) { player->isCrouched = false; player->transitionedToCrouch = false;  player->crouchTransTime = 0.0f; }




    // Settings for the Jump button, when you hold down the space key, you will jump high
    // Note: Until I could find another way to solve this problem, the first jump is going to equal the 2nd jump velocity
    // The 2nd jump is going to equal the 3rd jump velocity, and the 3rd jump is going to equal the first jump velocity
    // this is because if i press space while the player is on the ground, he will jump and the number of jumps index will go up by one
    // meaning that the when pressing space bar the program will already take on the next jumps value in mid-air
    // FirstJump -> numOfJumps++ -> SecondJump
    
    // do these things when pressed only once and not crouching
    
    if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && !spacePressed)
    {
        spacePressed = true;
    } else if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) && spacePressed && player->isGrounded) { spacePressed = false; }
    // if The jump key is held Down


    if (spacePressed = true && numOfJumps == 0 && !player->crouchActions.backflip && !player->isCrouched)
    {
            if (!player->isGrounded) player->animID = PLAYER_SUPER_JUMP_ID;        
            
            player->FirstJump(deltaTime);
            if (!didIterateToNextJump)
            {
                   didIterateToNextJump = true;
                   numOfJumps++;
                
            }  
    }

    if (spacePressed = true && numOfJumps == 1 && !player->crouchActions.backflip && !player->isCrouched)
    {
            if (!player->isGrounded) player->animID = PLAYER_JUMP_ID;
            player->SecondJump(deltaTime);
            if (!didIterateToNextJump)
            {

                didIterateToNextJump = true;
                numOfJumps++;
            }
    }
    if (spacePressed = true && numOfJumps == 2 && !player->crouchActions.backflip && !player->isCrouched)
    {
            if (!player->isGrounded) player->animID = PLAYER_SECOND_JUMP_ID;
            player->SuperJump(deltaTime);
            if (!didIterateToNextJump)
            {
                didIterateToNextJump = true;
                numOfJumps = 0;
            }
    }   
        

    if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && !player->isCrouched)
    {
        
        std::cout << numOfJumps;
        jumpKeyHeld = true;
        isJumping = true;
        jump_random_integer = jumpRand(mt);
        

        // First Jump Sound
        if (!jumpSoundPlayed && numOfJumps == 1)
        {
            std::cout << "Jump";
            SoundEngine->play2D(jump[jump_random_integer].c_str());
            jumpSoundPlayed = true;
            
        }

        // Second Jump Sound
        if (!jumpSoundPlayed && numOfJumps == 2)
        {
            std::cout << "Second Jump";
            SoundEngine->play2D("audio/Haugh!.wav");
            jumpSoundPlayed = true;
        } 
        // Third Jump Sound
        if (!jumpSoundPlayed && numOfJumps == 0)
        {
            std::cout << "Super Jump";
            SoundEngine->play2D("audio/Yahaa!!.wav");
            jumpSoundPlayed = true;
            
        }
  
        // Make Sure Jump Sound doesn't play twice, so this time after spacebar being more than 0.05 is here
        if (player->isGrounded && player->timeAfterSpacebar > 0.05)
        {
            jumpSoundPlayed = false;
            didIterateToNextJump = false;
            player->timeAfterSpacebar = 0.0f;
            return;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && !player->isGrounded)
    {
        jumpKeyHeld = false;
        isJumping = false;
        player->timeAfterSpacebar = 0.15f;
    }
    


}


void UpdateCrouchActions(GLFWwindow* window)
{
    
    if (player->isCrouched && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
         player->crouchActions.backflip = true;  
         // Make Sure Jump Sound doesn't play twice, so this time after spacebar being more than 0.05 is here
         if (player->isGrounded && player->timeAfterSpacebar > 0.05)
         {
             jumpSoundPlayed = false;
             didIterateToNextJump = false;
             player->timeAfterSpacebar = 0.0f;
             return;
         }
    }



    if (player->crouchActions.backflip)
        {
            if (!player->isGrounded) player->animID = PLAYER_BACK_JUMP_ID;
            std::cout << "Back-Flip";
            player->BackJump(deltaTime);
            if (player->isGrounded) { player->crouchActions.backflip = false; }
        }
    
}

void DoCollisions()
{
    bool isAnyColliderColliding = false;

   
    for (auto& floorCollider : floorColliders)
    {
        if (AABB(*player, floorCollider))
        {
            isAnyColliderColliding = true;
            break; // If the player is already colliding with one collider, no need to check others
        }
    }
    
    player->isGrounded = isAnyColliderColliding;
    
}

void GUI_SCENE(GLFWwindow* window)
{
    if (ImGui::Begin("Scene", NULL, gui.sceneFlags))
    {

        // Using a Child allow to fill all the space of the window.
        // It also alows customization
        if (ImGui::BeginTabBar("Our Scene"))
        {
            if (ImGui::BeginTabItem("Scene"))
            {
                if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused()) { gui.isSceneHovered = true; ImGui::CaptureMouseFromApp(false); processInput(window); UpdateCrouchActions(window); }
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