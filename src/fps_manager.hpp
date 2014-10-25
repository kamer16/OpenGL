#ifndef FPS_MANAGER_HPP
# define FPS_MANAGER_HPP

# include <GLFW/glfw3.h>
# include <string>

class fps_manager
{
public:
    static fps_manager& get_instance();
    void set_frequency(double frequency);
    // Needs to be called on each frame.  Without update_and_print_fps
    // returns -1 when not updated, or fps value when updated
    double update_fps();
    // A call to this functions much be done once per frame and without
    // update_fps
    void update_and_print_fps();
    void update_and_set_window_title(GLFWwindow* w, std::string&& window_name);
    double get_sec_per_frame();
private:
    fps_manager() {}
    double old_time_ = glfwGetTime();
    double fps_ = 0;
    double frame_count_ = 0;
    // Initial value when manager is launched instead of 0
    double sec_per_frame_ = 0.004;
    double frequency_ = 1;
    double old_time_sec_ = 0;
};

#endif // TIMER_HPP
