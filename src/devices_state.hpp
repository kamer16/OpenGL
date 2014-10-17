#ifndef DEVICES_STATE_HPP
# define DEVICES_STATE_HPP

#include <GLFW/glfw3.h>

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

struct devices_state
{
public:
    s_keyboard_state key_state;
    s_mouse_state mouse_state;
    // Returns the current state of the devices connected to window
    // Everytime an instance is called, the state of the device is updated
    static const devices_state &get_instance(GLFWwindow *window);

    void get_mouse_movement(double *xpos, double *ypos) const;

private:
    devices_state() {}
    devices_state(devices_state const&);
    void operator=(devices_state const&);
    void update_keyboard_state(GLFWwindow *window);
    void update_mouse_state(GLFWwindow *window);
};

#endif // DEVICES_STATE_HPP
