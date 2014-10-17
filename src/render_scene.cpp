#include <GL/glew.h>

#include "render_scene.hpp"

extern double g_x, g_y, g_z, g_angle_x, g_angle_y;

scene::scene(GLuint program_id, float aspect_ratio)
    : program_id_(program_id), aspect_ratio_(aspect_ratio),
      proj_mat_(glm::perspective(glm::radians(56.25f), aspect_ratio, 0.1f, 10000.0f)),
      view_mat_(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -70, -100.0), glm::vec3(0, 1, 0)))
{
}

void
scene::set_light_source()
{
  GLint light_dir_idx = glGetUniformLocation(program_id_, "light_dir");

  glm::vec3 light_dir(1, 0, 0.8);

  glUniform3fv(light_dir_idx, 1, glm::value_ptr(normal_mat_ * light_dir));
}

void
scene::set_material_color()
{
  GLint light_dir_idx = glGetUniformLocation(program_id_, "material.specular");

  glm::vec3 specular(1, 0, 0.8);

  glUniform3fv(light_dir_idx, 1, glm::value_ptr(specular));
}

void
scene::set_model_view_matrix()
{
  GLint projMatIdx = glGetUniformLocation(program_id_, "projMat");
  GLint model_view_idx = glGetUniformLocation(program_id_, "model_view_mat");
  GLint normal_mat_idx = glGetUniformLocation(program_id_, "normal_mat");

  glm::mat4 modelMat; // identity
  modelMat = glm::rotate(modelMat, static_cast<float>(g_angle_x), glm::vec3(1, 0, 0));
  modelMat = glm::rotate(modelMat, static_cast<float>(g_angle_y), glm::vec3(0, 1, 0));
  modelMat = glm::translate(modelMat, glm::vec3(g_x, g_y, g_z));

  glm::mat4 model_view_mat = view_mat_ * modelMat;
  normal_mat_ = glm::transpose(glm::inverse(glm::mat3(model_view_mat)));

  // When world moves, light moves with it
  glUniformMatrix4fv(projMatIdx, 1, GL_FALSE, glm::value_ptr(proj_mat_));
  glUniformMatrix4fv(model_view_idx, 1, GL_FALSE, glm::value_ptr(model_view_mat));
  glUniformMatrix3fv(normal_mat_idx, 1, GL_FALSE, glm::value_ptr(normal_mat_));
}

void
scene::render_arrays(GLuint *vaoID, int nb_elt)
{
  glBindVertexArray(vaoID[0]);
  glDrawArrays(GL_TRIANGLES, 0, nb_elt);
  glBindVertexArray(0);
}

void
scene::render_elements(GLuint *vaoID, int nb_elt)
{
  glBindVertexArray(vaoID[0]);
  glDrawElements(GL_TRIANGLES, nb_elt, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
