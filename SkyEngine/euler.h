#include <iostream>
#include <vector>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Euler
{
	public:
		glm::mat4 MVP = glm::mat4(1.0);
		std::vector<float> e_vertices;
		glm::vec3 startPoint;
		glm::vec3 endPoint;
		glm::vec3 lineColor;
};