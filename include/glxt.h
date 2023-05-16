#ifndef GLXT_H
#define GLXT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#if (defined(NDEBUG) && NDEBUG == 1) || !(defined(GLXT_RELEASE) && GLXT_RELEASE == 0)
    #define DEBUG_DO(STMT) STMT
#else
    #define DEBUG_DO(STMT)
#endif

bool glxt_has_failure(void);
const char* glxt_failure_reason(void);

uint32_t glxt_create_vertex_array(void);
void glxt_destroy_vertex_array(uint32_t vao);
void glxt_enable_vertex_array(uint32_t vao);
void glxt_disable_vertex_array(void);

void glxt_set_vertex_attrib(uint32_t index, int comp_count, int attr_type, 
    bool normalized, size_t vertex_size, const void *attr_offset);
void glxt_draw_vertex_array(int offset, int count);
void glxt_draw_vertex_array_elements(int offset, int count, const void* buffer);

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

enum {
    GLXT_NO_ERROR = 0,
    GLXT_OPENGL_INVALID_ENUM,
    GLXT_OPENGL_INVALID_OPERATION,
    GLXT_OPENGL_OUT_OF_MEMORY,
    GLXT_OPENGL_INVALID_FRAMEBUFFER_OPERATION,
};

struct {
    int last_opengl_error;
    int last_glxt_failure;
} GLXT = {0};

bool glxt_has_failure(void)
{
    return GLXT.last_opengl_error == GL_NO_ERROR;
}

const char* glxt_failure_reason(void)
{
    switch(GLXT.last_opengl_error) {
        case GL_INVALID_ENUM: return "OpenGL Error detected: GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "OpenGL Error detected: GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "OpenGL Error detected: GL_INVALID_OPERATION";
        case GL_OUT_OF_MEMORY: return "OpenGL Error detected: GL_OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "OpenGL Error detected: GL_INVALID_FRAMEBUFFER_OPERATION";
    }

    return "No Error Detected";
}

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

void glxt_set_vertex_attrib(uint32_t index, int comp_count, int attr_type, 
    bool normalized, size_t vertex_size, const void *attr_offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, comp_count, attr_type, normalized, vertex_size, attr_offset);
}

void glxt_draw_vertex_array(int offset, int count)
{
    glDrawArrays(GL_TRIANGLES, offset, count);
}

void glxt_draw_vertex_array_elements(int offset, int count, const void* buffer)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (const uint32_t*)buffer + offset);
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