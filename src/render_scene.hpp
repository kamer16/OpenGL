#ifndef RENDER_SCENE_CPP
# define RENDER_SCENE_CPP

#include <GL/glew.h> // GLuint

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
#include <glm/gtc/type_ptr.hpp> /* value_ptr */


class scene
{
public:
    scene(GLuint program_id, float aspect_ratio);
    void update();
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
    float aspect_ratio_;
    glm::mat3 normal_mat_;
    glm::mat4 proj_mat_;
    glm::mat4 view_mat_;

};

#endif // RENDER_SCENE_CPP
