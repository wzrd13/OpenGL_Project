#include "Mesh.h"
#include "VertexBufferLayout.h"

#include "glm/gtx/string_cast.hpp"
#include <iostream>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : VBO(&vertices[0], vertices.size() * sizeof(Vertex)), IBO(&indices[0], indices.size() * sizeof(unsigned int)), VAO()
{
    this->vertices = vertices;
    this->indices = indices;

    setupMesh();   
}

Mesh::Mesh(unsigned int count, const std::vector<unsigned int>& indices)
    : VBO(count * sizeof(Vertex)), IBO(&indices[0], indices.size() * sizeof(unsigned int)), VAO()
{
    this->vertices = vertices;
    this->indices = indices;

    setupMesh();
}

void Mesh::Draw(const Shader& shader)
{
    shader.Bind();
    VAO.Bind();
    IBO.Bind();
    glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Mesh::UpdateMesh(std::vector<Vertex> vertices)
{
    VBO.Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);
}

void Mesh::ToString()
{
    std::cout << "TO stirng" << std::endl;
    for (int i = 0; i < vertices.size(); i++)
    {
        std::cout << glm::to_string(vertices[i].Position) << std::endl;
        std::cout << glm::to_string(vertices[i].Color) << std::endl << std::endl;
    }
}

void Mesh::setupMesh()
{   
    VertexBufferLayout layout(sizeof(Vertex));
    layout.Push<float>(4, offsetof(Vertex, Position));
    layout.Push<float>(4, offsetof(Vertex, Color));
    layout.Push<float>(3, offsetof(Vertex, Normal));
    layout.Push<float>(2, offsetof(Vertex, TexCoords));

    VAO.AddBuffer(VBO, layout);
}
