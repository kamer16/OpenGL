#ifndef CAMERA_HPP
# define CAMERA_HPP

# define GLM_FORCE_RADIANS

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
# include <glm/gtc/type_ptr.hpp> /* value_ptr */

# include "devices_state.hpp"

class camera
{
public:
    camera(float aspect_ratio);
    glm::mat4& get_model_mat();
    glm::mat4& get_proj_mat();
    glm::mat4& get_view_mat();

    void update(const devices_state &device);
private:
    glm::mat4 proj_mat_;
    glm::mat4 view_mat_;
    glm::mat4 model_mat_;

    // Memorize global translation value to compute translation values
    glm::vec2 rotation_;

    // Update model matrix to move world around
    void update_position(const devices_state &device);
    void update_rotation(const devices_state &device);
};

#endif // CAMERA_HPP
