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

#if defined(GLXT_WITH_IO_HELPER) && GLXT_WITH_IO_HELPER == 1
    #include <stdio.h>
    size_t glxt_get_file_size(FILE* f);
    size_t glxt_get_file_size_path(const char* file_path);
    void glxt_read_file_data(FILE* f, char* writable);
    void glxt_read_file_data_path(const char* file_path, char* writable);
#endif

enum {
    GLXT_SHADER_UNIFORM_FLOAT = 0,
    GLXT_SHADER_UNIFORM_VEC2,
    GLXT_SHADER_UNIFORM_VEC3,
    GLXT_SHADER_UNIFORM_VEC4,
    GLXT_SHADER_UNIFORM_INT,
    GLXT_SHADER_UNIFORM_IVEC2,
    GLXT_SHADER_UNIFORM_IVEC3,
    GLXT_SHADER_UNIFORM_IVEC4,
    GLXT_SHADER_UNIFORM_SAMPLER2D,
};

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

uint32_t glxt_create_shader_program(const char* vert_source, const char* frag_source);
void glxt_destroy_shader_program(uint32_t shader_program);
void glxt_enable_shader_program(uint32_t shader_program);
void glxt_disable_shader_program(uint32_t shader_program);
void glxt_set_shader_uniform(uint32_t shader_program, const char* name, 
    const void* data, int uniform_type, int count);
void glxt_set_shader_uniform_mat4(uint32_t shader_program, const char* name, const void* data);

uint32_t glxt_create_texture2d(uint32_t width, uint32_t height, int comp, const uint8_t* data);
void glxt_destroy_texture2d(uint32_t texture);
void glxt_enable_texture2d(uint32_t texture);
void glxt_disable_texture2d(uint32_t texture);


#endif // GLXT_H

#ifndef GLXT_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>

enum {
    GLXT_NO_ERROR = 0,
    GLXT_INVALID_NULL_ARGUMENTS,
    GLXT_OPENGL_INVALID_ENUM,
    GLXT_OPENGL_INVALID_VALUE,
    GLXT_OPENGL_INVALID_OPERATION,
    GLXT_OPENGL_OUT_OF_MEMORY,
    GLXT_OPENGL_INVALID_FRAMEBUFFER_OPERATION,
    GLXT_UNKNOWN_UNIFORM_SHADER_TYPE,
    GLXT_UNIFORM_LOCATION_NOT_FOUND,
    GLXT_VERTEX_SHADER_COMPILATION_FAILURE,
    GLXT_FRAGMENT_SHADER_COMPILATION_FAILURE,
    GLXT_SHADER_PROGRAM_LINKING_FAILURE,

    GLXT_FAILED_TO_OPEN_FILE,
};

struct {
    int last_failure;
    uint32_t default_shader_program;
} GLXT = {0};

bool glxt_has_failure(void)
{
    return GLXT.last_failure != GLXT_NO_ERROR;
}

int _glxt_check_opengl_error(void)
{
    switch(glGetError()) {
        case GL_INVALID_ENUM: return GLXT_OPENGL_INVALID_ENUM;
        case GL_INVALID_VALUE: return GLXT_OPENGL_INVALID_VALUE;
        case GL_INVALID_OPERATION: return GLXT_OPENGL_INVALID_OPERATION;
        case GL_OUT_OF_MEMORY: return GLXT_OPENGL_OUT_OF_MEMORY;
        case GL_INVALID_FRAMEBUFFER_OPERATION: return GLXT_OPENGL_INVALID_FRAMEBUFFER_OPERATION;
        default: return GLXT_NO_ERROR;
    }
    return GLXT_NO_ERROR;
}

const char* glxt_failure_reason(void)
{
    switch(GLXT.last_failure) {
        case GLXT_NO_ERROR: return NULL;
        case GLXT_INVALID_NULL_ARGUMENTS: return "ERROR: Invalid null arguments";
        case GLXT_FAILED_TO_OPEN_FILE: return "ERROR: Failed to open a file";
        case GLXT_OPENGL_INVALID_ENUM: return "ERROR: GL_INVALID_ENUM";
        case GLXT_OPENGL_INVALID_VALUE: return "ERROR: GL_INVALID_VALUE";
        case GLXT_OPENGL_INVALID_OPERATION: return "ERROR: GL_INVALID_OPERATION";
        case GLXT_OPENGL_OUT_OF_MEMORY: return "ERROR: GL_OUT_OF_MEMORY";
        case GLXT_OPENGL_INVALID_FRAMEBUFFER_OPERATION: return "ERROR: GL_INVALID_FRAMEBUFFER_OPERATION";
        case GLXT_UNKNOWN_UNIFORM_SHADER_TYPE: return "ERROR: Unknown uniform shader type";
        case GLXT_UNIFORM_LOCATION_NOT_FOUND: return "ERROR: Uniform location not found";
        case GLXT_VERTEX_SHADER_COMPILATION_FAILURE: return "ERROR: Vertex shader compilation failure";
        case GLXT_FRAGMENT_SHADER_COMPILATION_FAILURE: return "ERROR: Fragment shader compilation failure";
        case GLXT_SHADER_PROGRAM_LINKING_FAILURE: return "ERROR: Shader program linking failure";
        default: return "Invalid error code detected";
    }
    return "Invalid error code detected";
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

uint32_t glxt_create_shader_program(const char* vert_source, const char* frag_source)
{
    // Create and compile the vertex shader
    int is_compiled;
    uint32_t vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_source, NULL);
    glCompileShader(vert_shader);

    is_compiled = 0;
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &is_compiled);
    if(is_compiled == GL_FALSE) {
        GLXT.last_failure = GLXT_VERTEX_SHADER_COMPILATION_FAILURE;
        DEBUG_DO(do {
            int error_log_length = 0;
            glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &error_log_length);
            char* err_msg = malloc(error_log_length * sizeof(char));
            glGetShaderInfoLog(vert_shader, error_log_length, &error_log_length, err_msg);
            fprintf(stderr, "%s\n", err_msg);
            free(err_msg);
        } while(0));
        glDeleteShader(vert_shader);
        return 0;
    }

    // Create and compile the fragment shader
    uint32_t frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_source, NULL);
    glCompileShader(frag_shader);

    is_compiled = 0;
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &is_compiled);
    if(is_compiled == GL_FALSE) {
        GLXT.last_failure = GLXT_FRAGMENT_SHADER_COMPILATION_FAILURE;
        DEBUG_DO(do {
            int error_log_length = 0;
            glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &error_log_length);
            char* err_msg = malloc(error_log_length * sizeof(char));
            glGetShaderInfoLog(frag_shader, error_log_length, &error_log_length, err_msg);
            fprintf(stderr, "%s\n", err_msg);
            free(err_msg);
        } while(0));
        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);
        return 0;
    }

    // Link the vertex and fragment shader into a shader program
    uint32_t shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);

    int is_linked = 0;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &is_linked);
    if(is_linked == GL_FALSE) {
        GLXT.last_failure = GLXT_SHADER_PROGRAM_LINKING_FAILURE;
        DEBUG_DO(do {
            int error_log_length = 0;
            glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &error_log_length);
            char* err_msg = malloc(error_log_length * sizeof(char));
            glGetProgramInfoLog(shader_program, error_log_length, &error_log_length, err_msg);
            fprintf(stderr, "%s\n", err_msg);
            free(err_msg);
        } while(0));
        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);
        return 0;
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    DEBUG_DO(_glxt_check_opengl_error());

    return shader_program;
}

void glxt_destroy_shader_program(uint32_t shader_program)
{
    glDeleteProgram(shader_program);
}

void glxt_enable_shader_program(uint32_t shader_program)
{
    glUseProgram(shader_program);
}

void glxt_disable_shader_program(uint32_t shader_program)
{
    glUseProgram(0);
}

void glxt_set_shader_uniform(uint32_t shader_program, const char* name, 
    const void* data, int uniform_type, int count)
{
    int location = -1;
    location = glGetUniformLocation(shader_program, name);
    if(location == -1)
        GLXT.last_failure = GLXT_UNIFORM_LOCATION_NOT_FOUND;

    switch(uniform_type)
    {
        case GLXT_SHADER_UNIFORM_FLOAT: glUniform1fv(location, count, (float*)data); break;
        case GLXT_SHADER_UNIFORM_VEC2: glUniform2fv(location, count, (float*)data); break;
        case GLXT_SHADER_UNIFORM_VEC3: glUniform3fv(location, count, (float*)data); break;
        case GLXT_SHADER_UNIFORM_VEC4: glUniform4fv(location, count, (float*)data); break;
        case GLXT_SHADER_UNIFORM_INT: glUniform1iv(location, count, (int*)data); break;
        case GLXT_SHADER_UNIFORM_IVEC2: glUniform2iv(location, count, (int*)data); break;
        case GLXT_SHADER_UNIFORM_IVEC3: glUniform3iv(location, count, (int*)data); break;
        case GLXT_SHADER_UNIFORM_IVEC4: glUniform4iv(location, count, (int*)data); break;
        case GLXT_SHADER_UNIFORM_SAMPLER2D: glUniform1iv(location, count, (int*)data); break;
        default:
            GLXT.last_failure = GLXT_UNKNOWN_UNIFORM_SHADER_TYPE;
    }

    DEBUG_DO(_glxt_check_opengl_error());
}

void glxt_set_shader_uniform_mat4(uint32_t shader_program, const char* name, const void* data)
{
    int location = -1;
    location = glGetUniformLocation(shader_program, name);
    if(location == -1)
        GLXT.last_failure = GLXT_UNIFORM_LOCATION_NOT_FOUND;
    glUniformMatrix4fv(location, 1, false, (const float*)data);
    
    DEBUG_DO(_glxt_check_opengl_error());
}

void glxt_read_file_data(FILE* f, char* writable)
{
    if(writable == NULL) {
        GLXT.last_failure = GLXT_INVALID_NULL_ARGUMENTS;
        return;
    }

    if(f == NULL) {
        GLXT.last_failure = GLXT_INVALID_NULL_ARGUMENTS;
        return;
    }

    size_t i = 0;
    char c;
    while(!feof(f)) {
        c = fgetc(f);
        writable[i] = c;
        ++i;
    }

    fseek(f, 0, SEEK_SET);
}

void glxt_read_file_data_path(const char* file_path, char* writable)
{
    if(writable == NULL) {
        GLXT.last_failure = GLXT_INVALID_NULL_ARGUMENTS;
        return;
    }

    FILE* f = fopen(file_path, "r");
    if(f == NULL) {
        GLXT.last_failure = GLXT_FAILED_TO_OPEN_FILE;
        return;
    }

    glxt_read_file_data(f, writable);
}

size_t glxt_get_file_size(FILE* f)
{
    if(f == NULL) {
        GLXT.last_failure = GLXT_INVALID_NULL_ARGUMENTS;
        return 0;
    }
    size_t size = 0;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

size_t glxt_get_file_size_path(const char* file_path)
{
    if(file_path == NULL) {
        GLXT.last_failure = GLXT_INVALID_NULL_ARGUMENTS;
        return 0;
    }

    FILE* f = fopen(file_path, "r");
    if(f == NULL) {
        GLXT.last_failure = GLXT_FAILED_TO_OPEN_FILE;
        return 0;
    }

    return glxt_get_file_size(f);
}


#endif // GLXT_IMPLEMENTATION