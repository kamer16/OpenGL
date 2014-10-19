#ifndef RENDER_SCENE_CPP
# define RENDER_SCENE_CPP

# include <GL/glew.h> // GLuint

# define GLM_FORCE_RADIANS

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
# include <glm/gtc/type_ptr.hpp> /* value_ptr */

# include <GLFW/glfw3.h>

# include "devices_state.hpp"

class scene
{
public:
    scene(GLuint program_id, float aspect_ratio, GLFWwindow *window);
    // Draw objects contained by the vertex array object
    void render_arrays(GLuint vao_id, int nb_elt);
    void render_elements(GLuint vao_id, int nb_elt);
    // Check for devices inputs, and update model matrix and light position
    // accordingly.
    // Set the shaders with the updated transformation matrix and light
    // position.
    void update(const devices_state &device);

private:
    // Update model matrix to move world around
    void update_position(const devices_state &device);
    void update_rotation(const devices_state &device);

    // Transforms light position into eye coordinate space and send it to shader
    // Activate ambiant, specular, diffuse parameter in shader
    void update_light_source();

    void update_model_view_matrix();
    // Activate ambiant, specular, diffuse parameter for light in shader
    void set_light_color();
    // Activate ambiant, specular, diffuse parameter for material in shader
    void set_material_color();

    GLuint program_id_;

    glm::mat3 normal_mat_;
    glm::mat4 proj_mat_;
    glm::mat4 view_mat_;

    glm::vec3 translation_;
    glm::vec2 rotation_;

    float padding_;
    GLFWwindow *window_;
};

#endif // RENDER_SCENE_CPP
