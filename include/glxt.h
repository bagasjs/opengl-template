#ifndef GLXT_H
#define GLXT_H

#include <stdint.h>
#include <stddef.h>

#if (defined(NDEBUG) && NDEBUG == 1) || !(defined(GLXT_RELEASE) && GLXT_RELEASE == 0)
    #define DEBUG_DO(STMT) STMT
#else
    #define DEBUG_DO(STMT)
#endif

uint32_t glxt_create_vertex_array(void);
void glxt_destroy_vertex_array(uint32_t vao);
void glxt_enable_vertex_array(uint32_t vao);
void glxt_disable_vertex_array(void);

uint32_t glxt_create_vertex_buffer(size_t buffer_size, const void* buffer_data);
void glxt_update_vertex_buffer(uint32_t vbo, size_t buffer_size, const void* buffer_data, int offset);
void glxt_destroy_vertex_buffer(uint32_t vbo);
void glxt_enable_vertex_buffer(uint32_t vbo);
void glxt_disable_vertex_buffer(void);

uint32_t glxt_create_index_buffer(size_t buffer_size, const void* buffer_data);
void glxt_update_index_buffer(uint32_t ibo, size_t buffer_size, const void* buffer_data, int offset);
void glxt_destroy_index_buffer(uint32_t ibo);
void glxt_enable_index_buffer(uint32_t ibo);
void glxt_disable_index_buffer(void);


#endif // GLXT_H

#ifndef GLXT_IMPLEMENTATION

#include <glad/glad.h>

uint32_t glxt_create_vertex_array(void)
{
    uint32_t vao = 0;
    glGenVertexArrays(1, &vao);
    return vao;
}

void glxt_destroy_vertex_array(uint32_t vao)
{
    glDeleteVertexArrays(1, &vao);
}

void glxt_enable_vertex_array(uint32_t vao)
{
    glBindVertexArray(vao);
}

void glxt_disable_vertex_array(void)
{
    glBindVertexArray(0);
}

uint32_t glxt_create_vertex_buffer(size_t buffer_size, const void* buffer_data)
{
    uint32_t vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, buffer_data, GL_DYNAMIC_DRAW);

    DEBUG_DO(glBindBuffer(GL_ARRAY_BUFFER, 0));
    return vbo;
}

void glxt_update_vertex_buffer(uint32_t vbo, size_t buffer_size, const void* buffer_data, int offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, offset, buffer_size, buffer_data);
    DEBUG_DO(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void glxt_destroy_vertex_buffer(uint32_t vbo)
{
    glDeleteBuffers(1, &vbo);
}

void glxt_enable_vertex_buffer(uint32_t vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void glxt_disable_vertex_buffer(void)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


uint32_t glxt_create_index_buffer(size_t buffer_size, const void* buffer_data)
{
    uint32_t ibo = 0;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_size, buffer_data, GL_DYNAMIC_DRAW);

    DEBUG_DO(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    return ibo;
}

void glxt_update_index_buffer(uint32_t ibo, size_t buffer_size, const void* buffer_data, int offset)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, buffer_size, buffer_data);
    DEBUG_DO(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void glxt_destroy_index_buffer(uint32_t ibo)
{
    glDeleteBuffers(1, &ibo);
}

void glxt_enable_index_buffer(uint32_t ibo)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void glxt_disable_index_buffer(void)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

#endif // GLXT_IMPLEMENTATION