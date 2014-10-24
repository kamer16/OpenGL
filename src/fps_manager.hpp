#ifndef FPS_MANAGER_HPP
# define FPS_MANAGER_HPP

# include <GLFW/glfw3.h>
# include <string>

class fps_manager
{
public:
    // Needs to be called on each frame.  Without update_and_print_fps
    // returns -1 when not updated, or fps value when updated
    double update_fps();
    // A call to this functions much be done once per frame and without
    // update_fps
    void update_and_print_fps();
    fps_manager(double frequency = 1);
    void update_and_set_window_title(GLFWwindow* w, std::string&& window_name);
private:
    double old_time_ = 0;
    double fps_ = 0;
    double frame_count_ = 0;
    double frequency_;
};

#endif // TIMER_HPP
