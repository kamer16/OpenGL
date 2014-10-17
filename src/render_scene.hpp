#ifndef RENDER_SCENE_CPP
# define RENDER_SCENE_CPP

# include <GL/glew.h> // GLuint

# define GLM_FORCE_RADIANS

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
# include <glm/gtc/type_ptr.hpp> /* value_ptr */


# include <GLFW/glfw3.h>

class scene
{
public:
    scene(GLuint program_id, float aspect_ratio, GLFWwindow *window);

    // Update model matrix to move world around
    void update_position();
    void update_rotation();

    void set_model_view_matrix();
    void render_arrays(GLuint *vaoID, int nb_elt);
    void render_elements(GLuint *vaoID, int nb_elt);

    // Activate ambiant, specular, diffuse parameter for material in shader
    void set_material_color();

    // Transforms light position into eye coordinate space and send it to shader
    // Activate ambiant, specular, diffuse parameter in shader
    void set_light_source();

private:
    GLuint program_id_;

    glm::mat3 normal_mat_;
    glm::mat4 proj_mat_;
    glm::mat4 view_mat_;

    glm::vec3 translation_;
    glm::vec2 rotation_;

    GLFWwindow *window_;
};

#endif // RENDER_SCENE_CPP
