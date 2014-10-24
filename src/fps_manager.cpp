#include "fps_manager.hpp"

#include <iostream>
#include <sstream>

fps_manager::fps_manager(double frequency)
    : old_time_(glfwGetTime()),
      fps_(0),
      frame_count_(0),
      frequency_(frequency)
{
}

double
fps_manager::update_fps()
{
    ++frame_count_;
    double new_time = glfwGetTime();
    double elapsed_time = new_time - old_time_;
    if (elapsed_time > frequency_) {
        fps_ = frame_count_ / elapsed_time;
        frame_count_ = 0;
        old_time_ = new_time;
        return fps_;
    }
    return -1;
}

void
fps_manager::update_and_print_fps()
{
    if (update_fps() > 0)
        std::cout << fps_ << std::endl;
}

void
fps_manager::update_and_set_window_title(GLFWwindow* w,
                                         std::string&& window_name)
{
    if (update_fps() > 0) {
        std::ostringstream stream;
        stream << fps_;
        std::string fps_string = stream.str();

        // Append the FPS value to the window title details
        std::string tempWindowTitle = window_name + " | FPS: " + fps_string;

        // Convert the new window title to a c_str and set it
        glfwSetWindowTitle(w, tempWindowTitle.c_str());
    }
}
