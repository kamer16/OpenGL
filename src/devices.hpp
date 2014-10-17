#ifndef DEVICES_HPP
# define DEVICES_HPP

#include <GLFW/glfw3.h>

void handle_keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);
void handle_mouse_position(GLFWwindow *w, double x, double y);

struct s_keyboard_state
{
    char up_pressed;
    char down_pressed;
    char right_pressed;
    char left_pressed;
    char w_pressed;
    char a_pressed;
    char s_pressed;
    char d_pressed;
};

struct s_mouse_state
{
    double y_axis;
    double x_axis;
};

struct devices
{
public:
    s_keyboard_state key_state;
    s_mouse_state mouse_state;
    static devices &get_instance();

private:
    devices() {}
    devices(devices const&);
    void operator=(devices const&);
};

#endif // DEVICES_HPP
