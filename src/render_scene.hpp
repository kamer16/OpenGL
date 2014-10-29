#ifndef RENDER_SCENE_CPP
# define RENDER_SCENE_CPP

# include <GL/glew.h> // GLuint
# include <vector>

# include <glm/glm.hpp>
# include <glm/gtc/type_ptr.hpp> /* value_ptr */

# include <GLFW/glfw3.h>

# include "devices_state.hpp"
# include "object.hpp"
# include "camera.hpp"
# include "program.hpp"

class scene
{
public:
    scene(GLuint program_id, float aspect_ratio);
    // Check for devices inputs, and update model matrix and light position
    // accordingly.
    // Set the shaders with the updated transformation matrix and light
    // position.
    // Draw each object
    void update_and_draw(const devices_state &device, program& program);
    void add_object(object *object);

private:
    // Transforms light position into eye coordinate space and send it to shader
    // Activate ambiant, specular, diffuse parameter in shader
    void set_light_source();

    // \param model_mat, the object's being drawn model matrix
    void set_model_view_matrix(const glm::mat4& model_mat);
    // Activate ambiant, specular, diffuse parameter for light in shader
    void set_light_color();

    GLuint program_id_;

    camera camera_;

    std::vector<object *> objects_;
};

#endif // RENDER_SCENE_CPP
