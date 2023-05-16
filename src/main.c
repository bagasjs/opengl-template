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

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glfwSwapBuffers(window);
    }

    glxt_destroy_vertex_array(vao);

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}