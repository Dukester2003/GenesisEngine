#version 450 core
out vec4 FragColor;
in vec4 vertexColor; // input variable from vs (same name and type)


void main()
{
    FragColor = vertexColor;
}
