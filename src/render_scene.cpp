#include <GL/glew.h>

#include "render_scene.hpp"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
#include <glm/gtc/type_ptr.hpp> /* value_ptr */

extern double g_x, g_y, g_z, g_angle_x, g_angle_y;
void set_model_view_matrix(GLuint program_id, float aspect_ratio)
{
  GLint modelMatIdx = glGetUniformLocation(program_id, "modelMat");
  GLint viewMatIdx = glGetUniformLocation(program_id, "viewMat");
  GLint projMatIdx = glGetUniformLocation(program_id, "projMat");

  glm::mat4 projMat = glm::perspective(56.25f, aspect_ratio, 0.1f, 50.0f);
  glm::mat4 viewMat = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-g_angle_x, 0, -1.0), glm::vec3(0, 1, 0));
  glm::mat4 modelMat; // identity

  modelMat = glm::translate(modelMat, glm::vec3(g_x, 0, g_z));
  //modelMat = glm::rotate(modelMat, angle_x, glm::vec3(0, 1, 0));
  //modelMat = glm::rotate(modelMat, angle_y, glm::vec3(1, 0, 0));

  //modelMat = glm::translate(modelMat, glm::vec3(0, 0, -6));

  glUniformMatrix4fv(projMatIdx, 1, GL_FALSE, glm::value_ptr(projMat));
  glUniformMatrix4fv(viewMatIdx, 1, GL_FALSE, glm::value_ptr(viewMat));
  glUniformMatrix4fv(modelMatIdx, 1, GL_FALSE, glm::value_ptr(modelMat));
}

void renderScene(GLuint *vaoID)
{
  glBindVertexArray(vaoID[0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}
