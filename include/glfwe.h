#ifndef GLFWE_H
#define GLFWE_H

#define GLFWE_MAXIMUM_FRAME_EVENT 64

typedef enum { FALSE = 0, TRUE, } BOOL;

typedef enum GLFWEEventType {
    GLFWE_EVENT_NONE = 0,
    GLFWE_EVENT_QUIT,
    GLFWE_EVENT_KEY_PRESSED,
    GLFWE_EVENT_KEY_RELEASED,
    GLFWE_EVENT_KEY_REPEATED,
    GLFWE_EVENT_CURSOR_MOVED,
    GLFWE_EVENT_CURSOR_ENTER_WINDOW,
    GLFWE_EVENT_CURSOR_LEAVE_WINDOW,
    GLFWE_EVENT_MOUSE_KEY_LEFT_UP,
    GLFWE_EVENT_MOUSE_KEY_LEFT_DOWN,
    GLFWE_EVENT_MOUSE_KEY_RIGHT_UP,
    GLFWE_EVENT_MOUSE_KEY_RIGHT_DOWN,
    GLFWE_EVENT_WINDOW_RESIZED,
    GLFWE_EVENT_WINDOW_FRAMEBUFFER_RESIZED,
    GLFWE_EVENT_WINDOW_MOVED,
    GLFWE_EVENT_WINDOW_FOCUS_GAINED,
    GLFWE_EVENT_WINDOW_FOCUS_LOST,
} GLFWEEventType;

typedef struct GLFWEEvent {
    int type;
    struct { int code; char printable; } key;
    struct { int x, y; } mouse;
    struct { int x, y, w, h; } window;
    struct { int w, h; } framebuffer;
} GLFWEEvent;

typedef struct GLFWwindow GLFWwindow;

BOOL glfwe_init(GLFWwindow* window);
BOOL glfwe_poll_events(GLFWEEvent* event);
void glfwe_events_flush(void);

#endif

#ifdef GLFWE_IMPLEMENTATION

#include <stddef.h>
#include <GLFW/glfw3.h>

#ifndef GLFWE_MAXIMUM_FRAME_EVENT
    #define GLFWE_MAXIMUM_FRAME_EVENT 64
#endif

static struct {
    BOOL initialized;
    GLFWwindow* window;
    size_t events_count;
    size_t events_iterator;
    GLFWEEvent events[GLFWE_MAXIMUM_FRAME_EVENT];
} GLFWE = {0};

static void glfwe_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(GLFWE.events_count + 1 > GLFWE_MAXIMUM_FRAME_EVENT) return;

    GLFWE.events[GLFWE.events_count].key.code = key;
    GLFWE.events[GLFWE.events_count].key.printable = '\0';
    if(key >= GLFW_KEY_SPACE && key <= GLFW_KEY_WORLD_2) {
        GLFWE.events[GLFWE.events_count].key.printable = (char)key;
    }


    if(action == GLFW_PRESS)
        GLFWE.events[GLFWE.events_count].type = GLFWE_EVENT_KEY_PRESSED;
    if(action == GLFW_RELEASE)
        GLFWE.events[GLFWE.events_count].type = GLFWE_EVENT_KEY_RELEASED;
    if(action == GLFW_REPEAT)
        GLFWE.events[GLFWE.events_count].type = GLFWE_EVENT_KEY_REPEATED;

    GLFWE.events_count += 1;
}
static void glfwe_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(GLFWE.events_count + 1 > GLFWE_MAXIMUM_FRAME_EVENT) return;

    GLFWE.events[GLFWE.events_count].mouse.x = (int)xpos;
    GLFWE.events[GLFWE.events_count].mouse.y = (int)ypos;
    GLFWE.events[GLFWE.events_count].type = GLFWE_EVENT_CURSOR_MOVED;
    GLFWE.events_count += 1;
}

static void glfwe_mouse_button_callback(
    GLFWwindow* window, int button, int action, int mods)
{
    if(GLFWE.events_count + 1 > GLFWE_MAXIMUM_FRAME_EVENT) return;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        GLFWE.events[GLFWE.events_count].type = GLFWE_EVENT_MOUSE_KEY_RIGHT_DOWN;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        GLFWE.events[GLFWE.events_count].type = GLFWE_EVENT_MOUSE_KEY_RIGHT_UP;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        GLFWE.events[GLFWE.events_count].type = GLFWE_EVENT_MOUSE_KEY_LEFT_DOWN;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        GLFWE.events[GLFWE.events_count].type = GLFWE_EVENT_MOUSE_KEY_LEFT_UP;

    GLFWE.events_count += 1;
}

void glfwe_window_size_callback(GLFWwindow* window, int width, int height)
{
    if(GLFWE.events_count + 1 > GLFWE_MAXIMUM_FRAME_EVENT) return;
    GLFWE.events[GLFWE.events_count].window.w = width;
    GLFWE.events[GLFWE.events_count].window.h = height;
    GLFWE.events[GLFWE.events_count].type = GLFWE_EVENT_WINDOW_RESIZED;
    GLFWE.events_count += 1;
}

void glfwe_window_pos_callback(GLFWwindow* window, int xpos, int ypos)
{
    if(GLFWE.events_count + 1 > GLFWE_MAXIMUM_FRAME_EVENT) return;
    GLFWE.events[GLFWE.events_count].window.x = xpos;
    GLFWE.events[GLFWE.events_count].window.y = ypos;
    GLFWE.events[GLFWE.events_count].type = GLFWE_EVENT_WINDOW_MOVED;
    GLFWE.events_count += 1;
}

void glfwe_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if(GLFWE.events_count + 1 > GLFWE_MAXIMUM_FRAME_EVENT) return;
    GLFWE.events[GLFWE.events_count].framebuffer.w = width;
    GLFWE.events[GLFWE.events_count].framebuffer.h = height;
    GLFWE.events[GLFWE.events_count].type = GLFWE_EVENT_WINDOW_FRAMEBUFFER_RESIZED;
    GLFWE.events_count += 1;
}

void glfwe_cursor_enter_callback(GLFWwindow* window, int enter) 
{
    if(GLFWE.events_count + 1 > GLFWE_MAXIMUM_FRAME_EVENT) return;

    if(enter) {
        GLFWE.events[GLFWE.events_count].type = GLFWE_EVENT_CURSOR_ENTER_WINDOW;
    } else {
        GLFWE.events[GLFWE.events_count].type = GLFWE_EVENT_CURSOR_LEAVE_WINDOW;
    }

    GLFWE.events_count += 1;
}

void glfwe_window_focus_callback(GLFWwindow* window, int focused)
{
    if(GLFWE.events_count + 1 > GLFWE_MAXIMUM_FRAME_EVENT) return;

    if(focused) {
        GLFWE.events[GLFWE.events_count].type = GLFWE_EVENT_WINDOW_FOCUS_GAINED;
    } else {
        GLFWE.events[GLFWE.events_count].type = GLFWE_EVENT_WINDOW_FOCUS_LOST;
    }

    GLFWE.events_count += 1;
}


BOOL glfwe_init(GLFWwindow* window)
{
    if(window == NULL) return FALSE;
    GLFWE.events_count = 0;
    GLFWE.events_iterator = 0;
    GLFWE.window = window;
    glfwSetKeyCallback(window, glfwe_key_callback);
    glfwSetCursorPosCallback(window, glfwe_cursor_pos_callback);
    glfwSetMouseButtonCallback(window, glfwe_mouse_button_callback);
    glfwSetWindowSizeCallback(window, glfwe_window_size_callback);
    glfwSetWindowPosCallback(window, glfwe_window_pos_callback);
    glfwSetFramebufferSizeCallback(window, glfwe_framebuffer_size_callback);
    glfwSetCursorEnterCallback(window, glfwe_cursor_enter_callback);
    glfwSetWindowFocusCallback(window, glfwe_window_focus_callback);
    return TRUE;
}

void glfwe_events_flush(void)
{
    GLFWE.events_count = 0;
    GLFWE.events_iterator = 0;
    glfwWaitEvents();
}

BOOL glfwe_poll_events(GLFWEEvent* event)
{
    if(GLFWE.events_iterator > GLFWE.events_count) return FALSE;

    *event = GLFWE.events[GLFWE.events_iterator];
    GLFWE.events_iterator += 1;
    return TRUE;
}


#endif // GLFWE_IMPLEMENTATION