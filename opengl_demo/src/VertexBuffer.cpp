#include "VertexBuffer.h"
#include "Renderer.h"
#include <iostream>

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &m_Renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(unsigned int size)
{
    glGenBuffers(1, &m_Renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_id);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_Renderer_id);
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_id);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
