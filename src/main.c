/**
 * If you're going to implement glxt in the file where you include glfw,
 * you should include glxt and create the implentation first then you include glfw
 */
#define GLXT_IMPLENTATION 
#include "glxt.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "OpenGL Template"

int main(int argc, char** argv)
{
    if(!glfwInit()) {
        fprintf(stderr, "%s\n", "Failed to initialize GLFW");
        return 0;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if(window == NULL) {
        fprintf(stderr, "%s\n", "Failed to create GLFW window");
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);
    gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);

    uint32_t vao = glxt_create_vertex_array();
    glxt_enable_vertex_array(vao);

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glfwSwapBuffers(window);
    }

    glxt_destroy_vertex_array(vao);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}