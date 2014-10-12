#include <GL/glew.h>
#include <SOIL/SOIL.h>

#include "texture.hpp"

void loadTextures(GLuint program_id)
{
  int width;
  int height;
  int channel;
  unsigned char *image;
  GLuint textures[2];
  glGenTextures(2, textures);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  image = SOIL_load_image("tree.jpg", &width, &height, &channel, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB ,GL_UNSIGNED_BYTE, image);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  GLint loc0 = glGetUniformLocation(program_id, "texture0");
  glUniform1i(loc0, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  image = SOIL_load_image("canion.jpg", &width, &height, &channel, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB ,GL_UNSIGNED_BYTE, image);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  GLint loc1 = glGetUniformLocation(program_id, "texture1");
  glUniform1i(loc1, 1);
}

