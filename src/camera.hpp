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
    const glm::mat4& get_proj_mat();
    const glm::mat4& get_view_mat();

    void update(const devices_state &device);
private:
    glm::mat4 proj_mat_;
    glm::mat4 view_mat_;

    // Camera's world coordinate position
    glm::vec3 position_;
    // Camera's spherical coordinates i-e horizontal, vertical angles
    glm::vec2 rotation_;

    // The camera's z axis and direction being looked at
    glm::vec3 forward_;
    // the camera's x axis
    glm::vec3 right_;
    // the camera's y axis
    glm::vec3 up_;

    // windows width ratio to windows height
    float aspect_ratio_;
    // Field of view of camera which can simulate zooming
    float fov_;

    // Update model matrix to move world around
    void update_position(const devices_state &device);
    // Update model matrix to move world around
    void update_rotation(const devices_state &device);

    float pos_incr_ = 500.0f;
    float rot_incr_ = 2.0f;
};

#endif // CAMERA_HPP
