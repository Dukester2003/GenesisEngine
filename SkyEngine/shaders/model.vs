#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 7) in vec4 aColor;

out vec2 TexCoords;
out vec4 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;   
    Color = aColor; 
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
