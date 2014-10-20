#include <GL/glew.h>

#include "render_scene.hpp"

scene::scene(GLuint program_id, float aspect_ratio)
    : program_id_(program_id),
      camera_(aspect_ratio)
{
    (void) padding_;
    glUseProgram(program_id_);
    set_light_color();
    set_material_color();
    glUseProgram(0);
}

void
scene::update_and_draw(const devices_state &device)
{
    camera_.update(device);
    set_model_view_matrix();
    set_light_source();
    for (auto obj : objects_)
        obj->draw();
}

void
scene::set_light_source()
{
    // create directional light
    GLint light_dir_idx = glGetUniformLocation(program_id_, "light.position");
    glm::vec3 light_dir(0, -0.1, 0.8);
    // When world moves, lights direction moves with it, therefore we multiply
    // it by a normal matrix.
    glUniform3fv(light_dir_idx, 1, glm::value_ptr(glm::normalize(light_dir)));
}

void scene::set_light_color()
{
    GLint specular_idx = glGetUniformLocation(program_id_, "light.param.specular");
    glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
    glUniform4fv(specular_idx, 1, glm::value_ptr(specular));

    glm::vec4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
    GLint diffuse_idx = glGetUniformLocation(program_id_, "light.param.diffuse");
    glUniform4fv(diffuse_idx, 1, glm::value_ptr(diffuse));

    glm::vec4 ambient(0.0f, 0.0f, 0.0f, 1.0f);
    GLint ambient_idx = glGetUniformLocation(program_id_, "light.param.ambient");
    glUniform4fv(ambient_idx, 1, glm::value_ptr(ambient));

    glm::vec4 g_ambient(0.2f, 0.2f, 0.2f, 1.0f);
    GLint g_ambient_idx = glGetUniformLocation(program_id_, "global_ambient");
    glUniform4fv(g_ambient_idx, 1, glm::value_ptr(g_ambient));
}

void
scene::set_material_color()
{
    GLint specular_idx = glGetUniformLocation(program_id_, "material.specular");
    glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);
    glUniform4fv(specular_idx, 1, glm::value_ptr(specular));

    glm::vec4 diffuse(0.8f, 0.8f, 0.8f, 1.0f);
    GLint diffuse_idx = glGetUniformLocation(program_id_, "material.diffuse");
    glUniform4fv(diffuse_idx, 1, glm::value_ptr(diffuse));

    glm::vec4 ambient(0.5f, 0.5f, 0.5f, 1.0f);
    GLint ambient_idx = glGetUniformLocation(program_id_, "material.ambient");
    glUniform4fv(ambient_idx, 1, glm::value_ptr(ambient));

    float shininess = 30.0f;
    GLint shininess_idx = glGetUniformLocation(program_id_, "material.shininess");
    glUniform1f(shininess_idx, shininess);
}

void
scene::set_model_view_matrix()
{
    GLint projMatIdx = glGetUniformLocation(program_id_, "projMat");
    GLint model_view_idx = glGetUniformLocation(program_id_, "model_view_mat");
    GLint normal_mat_idx = glGetUniformLocation(program_id_, "normal_mat");

    glm::mat4 model_mat;// = camera_.get_model_mat();
    const glm::mat4& view_mat = camera_.get_view_mat();
    const glm::mat4& proj_mat = camera_.get_proj_mat();
    glm::mat4 model_view_mat = view_mat * model_mat;
    glm::mat3 normal_mat = glm::transpose(glm::inverse(glm::mat3(model_view_mat)));

    glUniformMatrix4fv(projMatIdx, 1, GL_FALSE, glm::value_ptr(proj_mat));
    glUniformMatrix4fv(model_view_idx, 1, GL_FALSE, glm::value_ptr(model_view_mat));
    glUniformMatrix3fv(normal_mat_idx, 1, GL_FALSE, glm::value_ptr(normal_mat));
}

void
scene::render_arrays(GLuint vao_id, int nb_elt)
{
    glBindVertexArray(vao_id);
    glDrawArrays(GL_TRIANGLES, 0, nb_elt);
    glBindVertexArray(0);
}

void
scene::render_elements(GLuint vao_id, int nb_elt)
{
    glBindVertexArray(vao_id);
    glDrawElements(GL_TRIANGLES, nb_elt, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void
scene::add_object(polygon *object)
{
    objects_.push_back(object);
}
