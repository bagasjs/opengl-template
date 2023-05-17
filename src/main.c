/**
 * If you're going to implement glxt in the file where you include glfw,
 * you should include glxt and create the implentation first then you include glfw
 */
#define GLXT_IMPLENTATION 
#define GLXT_WITH_IO_HELPER 1
#include "glxt.h"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "OpenGL Template"
#define VERT_SHADER_SOURCE_PATH "./src/main.vert"
#define FRAG_SHADER_SOURCE_PATH "./src/main.frag"

#define GLXT_CHECK_ERROR() do {\
    if(glxt_has_failure()) { \
        fprintf(stderr, "%s\n", glxt_failure_reason()); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

typedef struct {
    struct { float x, y; } pos;
    struct { float r, g, b, a; } color;
} Vertex;

#define MAXIMUM_VERTICES 1000
static Vertex vertices[MAXIMUM_VERTICES] = {
    { .pos = { .x =  0.0f, .y =  0.5f }, .color = { 0.5f, 0.2f, 0.8f, 1.0f } },
    { .pos = { .x = -0.5f, .y = -0.5f }, .color = { 0.2f, 0.8f, 0.5f, 1.0f } },
    { .pos = { .x =  0.5f, .y = -0.5f }, .color = { 0.8f, 0.5f, 0.2f, 1.0f } },
    // { .pos = { .x = 0.0f, .y = 1.0f }, .color = { 0.2f, 0.5f, 0.8f, 1.0f } },
};
static size_t vertices_count = 3;

int main(int argc, char** argv)
{
    if(!glfwInit()) {
        fprintf(stderr, "%s\n", "Failed to initialize GLFW");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if(window == NULL) {
        fprintf(stderr, "%s\n", "Failed to create GLFW window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);

    uint32_t vao = glxt_create_vertex_array();
    glxt_enable_vertex_array(vao);

    FILE* vf = fopen(VERT_SHADER_SOURCE_PATH, "r");
    if(vf == NULL) {
        fprintf(stderr, "%s\n", "Failed to open vertex shader source file");
        exit(EXIT_FAILURE);
    }

    FILE* ff = fopen(FRAG_SHADER_SOURCE_PATH, "r");
    if(vf == NULL) {
        fprintf(stderr, "%s\n", "Failed to open fragment shader source file");
        exit(EXIT_FAILURE);
    }

    size_t vfsize = glxt_get_file_size(vf);
    size_t ffsize = glxt_get_file_size(ff);
    char* vert_source = malloc(sizeof(char) * vfsize);
    char* frag_source = malloc(sizeof(char) * ffsize);

    glxt_read_file_data(vf, vert_source);
    GLXT_CHECK_ERROR();

    glxt_read_file_data(ff, frag_source);
    GLXT_CHECK_ERROR();

    uint32_t shader_program =  glxt_create_shader_program(vert_source, frag_source);
    GLXT_CHECK_ERROR();

    uint32_t vbo = glxt_create_vertex_buffer(MAXIMUM_VERTICES, (const void*)vertices);
    glxt_enable_vertex_buffer(vbo);
    glxt_set_vertex_attrib(0, 2, GL_FLOAT, false, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
    glxt_set_vertex_attrib(1, 4, GL_FLOAT, false, sizeof(Vertex), (const void*)offsetof(Vertex, color));

    float a = 0.0f;
    float velocity = 0.01f;

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glxt_enable_shader_program(shader_program);

        glxt_set_shader_uniform(shader_program, "u_random_color", 
        (const void*)&a, GLXT_SHADER_UNIFORM_FLOAT, 1);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
        glfwSwapBuffers(window);

        a += velocity;
        if(a >= 1.0f || a <= 0.0f)
            velocity *= -1;
    }

    glxt_destroy_vertex_array(vao);

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}