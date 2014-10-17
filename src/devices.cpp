#include "devices.hpp"

devices &devices::get_instance()
{
    static devices instance;
    return instance;
}

void handle_keyboard(GLFWwindow *w, int key, int scancode, int action, int mods)
{
    devices &instance = devices::get_instance();
    (void) w;
    (void) key;
    (void) mods;
    if (action != GLFW_PRESS && action != GLFW_RELEASE)
        return;
    // Left arraw
    if (scancode == 113) {
        instance.key_state.left_pressed = action == GLFW_PRESS;
    }
    // Right arraw
    else if (scancode == 114) {
        instance.key_state.right_pressed = action == GLFW_PRESS;
    }
    // Up arraw
    else if (scancode == 111) {
        instance.key_state.up_pressed = action == GLFW_PRESS;
    }
    // Down arraw
    else if (scancode == 116) {
        instance.key_state.down_pressed = action == GLFW_PRESS;
    }
    // W key
    else if (scancode == 25) {
        instance.key_state.w_pressed = action == GLFW_PRESS;
    }
    // S key
    else if (scancode == 39) {
        instance.key_state.s_pressed = action == GLFW_PRESS;
    }
    // A key
    else if (scancode == 38) {
        instance.key_state.a_pressed = action == GLFW_PRESS;
    }
    // D key
    else if (scancode == 40) {
        instance.key_state.d_pressed = action == GLFW_PRESS;
    }
}

void handle_mouse_position(GLFWwindow *w, double x, double y)
{
    static double old_x = x;
    static double old_y = y;
    int height, width;

    glfwGetWindowSize(w, &width, &height);

    devices &instance = devices::get_instance();
    instance.mouse_state.x_axis += (x - old_x) / (width);
    instance.mouse_state.y_axis += (y - old_y) / (height);

    old_x = x;
    old_y = y;
}
