#version 450 core

in vec2 position;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main(void) {

	gl_Position = projection * view * model * vec4(position.x, 0.0, position.y, 1.0);
	TexCoords = vec2(position.x/2.0 + 0.5, position.y/2.0 + 0.5);
}