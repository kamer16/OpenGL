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
# include "material.hpp"

class scene
{
public:
    using materials = std::vector<material*>;
    scene(GLuint program_id, float aspect_ratio);
    // Draw objects contained by the vertex array object
    // TODO this class should not render objects, objects should draw themselves
    // with a draw call to the object
    void render_arrays(GLuint vao_id, int nb_elt);
    void render_elements(GLuint vao_id, int nb_elt);
    // Check for devices inputs, and update model matrix and light position
    // accordingly.
    // Set the shaders with the updated transformation matrix and light
    // position.
    // Draw each object
    void update_and_draw(const devices_state &device);
    void add_object(object *object);
    // Used to load vector which is currently created from mesh_loader
    void set_materials(std::vector<material*>* mats);

private:
    // Transforms light position into eye coordinate space and send it to shader
    // Activate ambiant, specular, diffuse parameter in shader
    void set_light_source();

    // \param model_mat, the object's being drawn model matrix
    void set_model_view_matrix(const glm::mat4& model_mat);
    // Activate ambiant, specular, diffuse parameter for light in shader
    void set_light_color();
    // Activate ambiant, specular, diffuse parameter for material in shader
    void set_material_color();

    GLuint program_id_;

    camera camera_;

    float padding_;
    std::vector<object *> objects_;
    materials* materials_;
};

#endif // RENDER_SCENE_CPP
