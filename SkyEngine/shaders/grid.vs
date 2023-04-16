#version 450 core
layout (location = 0) in vec3 gPos;
layout (location = 1) in vec4 aColor; // color has attribute position 1

out vec4 vertexColor;

uniform mat4 grid;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * grid * vec4(gPos, 1.0f);
    vertexColor = vec4(0.42f, 0.42f, 0.42f, 1.0f); // set vertexColor to input color from the vertex data

    if(gPos.z > 499.9999f && gPos.z < 500.0001f)
    {
        vertexColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }


    if(gPos.x > 499.9999f && gPos.x < 500.0001f)
    {
        vertexColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    }

    if((gPos.x > 499.9999f && gPos.z > 499.9999f) && (gPos.x < 500.0001f && gPos.z < 500.0001f))
    {
        vertexColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    }
}