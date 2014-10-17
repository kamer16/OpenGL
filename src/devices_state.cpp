#include "devices_state.hpp"

const devices_state &devices_state::get_instance(GLFWwindow *window)
{
    static devices_state instance;
    instance.update_keyboard_state(window);
    instance.update_mouse_state(window);
    return instance;
}

void
devices_state::update_keyboard_state(GLFWwindow *w)
{
    // Left arraw
    key_state.left_pressed = glfwGetKey(w, GLFW_KEY_LEFT) == GLFW_PRESS;

    // Right arraw
    key_state.right_pressed = glfwGetKey(w, GLFW_KEY_RIGHT) == GLFW_PRESS;

    // Up arraw
    key_state.up_pressed = glfwGetKey(w, GLFW_KEY_UP) == GLFW_PRESS;

    // Down arraw
    key_state.down_pressed = glfwGetKey(w, GLFW_KEY_DOWN) == GLFW_PRESS;

    // W key
    key_state.w_pressed = glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS;
    key_state.w_pressed |= glfwGetKey(w, GLFW_KEY_COMMA) == GLFW_PRESS;

    // S key
    key_state.s_pressed = glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS;
    key_state.s_pressed |= glfwGetKey(w, GLFW_KEY_O) == GLFW_PRESS;

    // A key
    key_state.a_pressed = glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS;

    // D key
    key_state.d_pressed = glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS;
    key_state.d_pressed |= glfwGetKey(w, GLFW_KEY_E) == GLFW_PRESS;
}

void
devices_state::get_mouse_movement(double *xpos, double *ypos) const
{
    *xpos = mouse_state.x_axis;
    *ypos = mouse_state.y_axis;
}

void
devices_state::update_mouse_state(GLFWwindow *window)
{
    static double old_x = 0;
    static double old_y = 0;
    int height, width;
    // Called each time in case size of window changes
    glfwGetWindowSize(window, &width, &height);
    glfwGetCursorPos(window, &mouse_state.x_axis, &mouse_state.y_axis);

    mouse_state.x_axis = (mouse_state.x_axis - old_x) / (width);
    mouse_state.y_axis = (mouse_state.y_axis - old_y) / (height);

    old_x = mouse_state.x_axis;
    old_y = mouse_state.y_axis;
}
