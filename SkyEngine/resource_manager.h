#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "g_model.h"
#include "g_shader.h"
#include "g_animation.h"

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no 
// public constructor is defined.
class ResourceManager
{
public:
    // resource storage
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Model> Models;
    static std::map<std::string, Animation> Animations;
    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader    LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    // retrieves a stored sader
    static Shader    GetShader(std::string name);
    // loads (and generates) a texture from file
    static Model LoadModel(const char* file, std::string name) {
        Models[name] = loadModelFromFile(file);
        return Models[name];
    }
    // retrieves a stored texture
    static Model GetModel(std::string name) {
        return Models[name];
    }

    static Animation LoadAnimation(const char* file, std::string name) {
        Animations[name] = loadAnimationFromFile(file);
        return Animations[name];
    }

    static Animation GetAnimation(std::string name) {
        return Animations[name];
    }
    // properly de-allocates all loaded resources
    static void      Clear();
private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { }
    // loads and generates a shader from file
    static Shader    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    static Model     loadModelFromFile(const char* ModelFile);
    static Animation loadAnimationFromFile(const char* AnimationFile);
};

#endif // !RESOURCE_MANAGER_H


