#ifndef RENDER_SCENE_CPP
# define RENDER_SCENE_CPP

# include <GL/glew.h> // GLuint
# include <vector>

# define GLM_FORCE_RADIANS

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
# include <glm/gtc/type_ptr.hpp> /* value_ptr */

# include <GLFW/glfw3.h>

# include "devices_state.hpp"
# include "polygon.hpp"
# include "camera.hpp"

class scene
{
public:
    scene(GLuint program_id, float aspect_ratio);
    // Draw objects contained by the vertex array object
    void render_arrays(GLuint vao_id, int nb_elt);
    void render_elements(GLuint vao_id, int nb_elt);
    // Check for devices inputs, and update model matrix and light position
    // accordingly.
    // Set the shaders with the updated transformation matrix and light
    // position.
    // Draw each object
    void update_and_draw(const devices_state &device);
    void add_object(polygon *object);

private:
    // Transforms light position into eye coordinate space and send it to shader
    // Activate ambiant, specular, diffuse parameter in shader
    void set_light_source();

    void set_model_view_matrix();
    // Activate ambiant, specular, diffuse parameter for light in shader
    void set_light_color();
    // Activate ambiant, specular, diffuse parameter for material in shader
    void set_material_color();

    GLuint program_id_;

    camera camera_;

    float padding_;
    std::vector<polygon *> objects_;
};

#endif // RENDER_SCENE_CPP
