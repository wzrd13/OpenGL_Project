#pragma once

#include "Shader.h"
#include "Renderer.h"

#include <vector>
#include <string>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec4 Position;
    glm::vec4 Color;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture_stuct {
    unsigned int id;
    std::string type;
};

class Mesh {
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture_stuct> textures;

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    Mesh(unsigned int size, const std::vector<unsigned int>& indices);
    void Draw(const Shader& shader);
    void UpdateMesh(std::vector<Vertex> vertices);
    void ToString();
private:
    //  render data
    VertexArray VAO;
    VertexBuffer VBO;
    IndexBuffer IBO;

    void setupMesh();
    void Light();
};

