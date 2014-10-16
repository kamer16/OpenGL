#include <GL/glew.h>

#include "render_scene.hpp"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
#include <glm/gtc/type_ptr.hpp> /* value_ptr */

#include <stdio.h>
extern double g_x, g_y, g_z, g_angle_x, g_angle_y;
void set_model_view_matrix(GLuint program_id, float aspect_ratio)
{
  GLint modelMatIdx = glGetUniformLocation(program_id, "modelMat");
  GLint viewMatIdx = glGetUniformLocation(program_id, "viewMat");
  GLint projMatIdx = glGetUniformLocation(program_id, "projMat");

  GLint model_view_idx = glGetUniformLocation(program_id, "model_view_mat");
  GLint normal_mat_idx = glGetUniformLocation(program_id, "normal_mat");

  glm::mat4 projMat = glm::perspective(glm::radians(56.25f), aspect_ratio, 0.1f, 10000.0f);
  glm::mat4 viewMat = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -6.0),
                                  glm::vec3(0, 1, 0));

  glm::mat4 modelMat; // identity
  modelMat = glm::rotate(modelMat, static_cast<float>(g_angle_y), glm::vec3(1, 0, 0));
  modelMat = glm::rotate(modelMat, static_cast<float>(g_angle_x), glm::vec3(0, 1, 0));
  modelMat = glm::translate(modelMat, glm::vec3(g_x, g_y, g_z));

  glm::mat4 model_view_mat = viewMat * modelMat;
  glm::mat3 normal_mat = glm::transpose(glm::inverse(glm::mat3(model_view_mat)));

  glUniformMatrix4fv(projMatIdx, 1, GL_FALSE, glm::value_ptr(projMat));
  glUniformMatrix4fv(viewMatIdx, 1, GL_FALSE, glm::value_ptr(viewMat));
  glUniformMatrix4fv(modelMatIdx, 1, GL_FALSE, glm::value_ptr(modelMat));
  glUniformMatrix4fv(model_view_idx, 1, GL_FALSE, glm::value_ptr(model_view_mat));
  glUniformMatrix4fv(normal_mat_idx, 1, GL_FALSE, glm::value_ptr(normal_mat));
}

void render_arrays(GLuint *vaoID, int nb_elt)
{
  glBindVertexArray(vaoID[0]);
  glDrawArrays(GL_TRIANGLES, 0, nb_elt);
  glBindVertexArray(0);
}

void render_elements(GLuint *vaoID, int nb_elt)
{
  glBindVertexArray(vaoID[0]);
  glDrawElements(GL_TRIANGLES, nb_elt, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
