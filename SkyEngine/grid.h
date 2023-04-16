#ifndef GRID_H
#define GRID_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<vector>
#include "g_shader.h"
#include "common_assets.h"

class Grid
{
    
public:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    // grid slices
    int slices = 1000;
    int gridLength = 1000;
    glm::vec3 gridOriginOffset = glm::vec3(-slices / 2, 0, -slices / 2);
    bool gridActive;
    void CreateGrid()
    {
        // Grid Vertex Data
        std::vector<glm::vec3> vertices;
        std::vector<glm::uvec4> indices;

        // i = x, j = z
        for (int j = 0; j <= slices; ++j) {
            for (int i = 0; i <= slices; ++i) {
                float x = (float)i / (float)slices;
                float y = 0;
                float z = (float)j / (float)slices;
                vertices.push_back(glm::vec3(x * 1000, y * 10, z * 1000));
            }
        }
        for (int j = 0; j < slices; ++j) {
            for (int i = 0; i < slices; ++i) {

                int row1 = j * (slices + 1);
                int row2 = (j + 1) * (slices + 1);

                indices.push_back(glm::uvec4(row1 + i, row1 + i + 1, row1 + i + 1, row2 + i + 1));
                indices.push_back(glm::uvec4(row2 + i + 1, row2 + i, row2 + i, row1 + i));

            }
        }

        // Vertex Array Objects
        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Vertex Buffoer Objects
        
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), glm::value_ptr(vertices[0]), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


        
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec4), glm::value_ptr(indices[0]), GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        gridLength = (GLuint)indices.size() * 4;


        glEnable(GL_DEPTH_TEST);
    }
    void RenderGrid()
    {
        // Grid Shader
        gridShader.use();
        glm::mat4 gridMat = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        // retrieve the matrix uniform locations
        unsigned int gridLoc = glGetUniformLocation(gridShader.ID, "grid");
        unsigned int viewLoc = glGetUniformLocation(gridShader.ID, "view");
        gridMat = glm::translate(gridMat, gridOriginOffset);
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(gridLoc, 1, GL_FALSE, glm::value_ptr(gridMat));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        // render container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    }

    void Deallocate()
    {
        glDeleteBuffers(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};



#endif // !GRID_H

