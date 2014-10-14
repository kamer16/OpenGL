#include <GL/glew.h>

#include "render_scene.hpp"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
#include <glm/gtc/type_ptr.hpp> /* value_ptr */

void renderScene(GLuint program_id, GLuint *vaoID)
{
  GLuint modelMatIdx = glGetUniformLocation(program_id, "modelMat");
  GLuint viewMatIdx = glGetUniformLocation(program_id, "viewMat");
  GLuint projMatIdx = glGetUniformLocation(program_id, "projMat");

  glm::mat4 modelMat; // identity
  glm::mat4 projMat = glm::perspective(56.25f, 640.0f/480.0f, 0.1f, 100.0f);
  glm::mat4 viewMat = glm::lookAt(glm::vec3(0, 6, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

  glUniformMatrix4fv(projMatIdx, 1, GL_FALSE, glm::value_ptr(projMat));
  glUniformMatrix4fv(viewMatIdx, 1, GL_FALSE, glm::value_ptr(viewMat));
  glUniformMatrix4fv(modelMatIdx, 1, GL_FALSE, glm::value_ptr(modelMat));

  glBindVertexArray(vaoID[0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  glBindVertexArray(vaoID[1]);
  glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}
