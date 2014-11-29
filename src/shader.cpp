#include <stdio.h> // printf

#include "textfile.hpp"
#include "shader.hpp"

void printShaderInfoLog(GLuint obj, const char *file)
{
  int infologLength = 0;
  int charsWritten  = 0;
  char *infoLog;

  glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

  if (infologLength > 1)
    {
      infoLog = new char[infologLength];
      glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
      printf("%s shader Log:%s\n",file, infoLog);
      delete[] infoLog;
    }
}

static void printProgramInfoLog(GLuint obj)
{
  int infologLength = 0;
  int charsWritten  = 0;
  char *infoLog;

  glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

  if (infologLength > 1)
    {
      infoLog = new char[infologLength];
      glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
      printf("Program Log:%s\n", infoLog);
      delete[] infoLog;
    }
}

GLuint attach_shader(const char *shader_file, GLenum shader_type,
                     GLuint program_id)
{
  char *shader_string = NULL;

  GLuint shader = glCreateShader(shader_type);

  shader_string = textFileRead(shader_file);

  glShaderSource(shader, 1, &shader_string, NULL);
  delete[] shader_string;

  glCompileShader(shader);

  printShaderInfoLog(shader, shader_file);

  glAttachShader(program_id, shader);

  return shader;
}

void destroy_shader(GLuint shader, GLuint program_id)
{
  glDetachShader(program_id, shader);
  glDeleteShader(shader);
}

void load_shaders(const char *vert_file, const char *frag_file,
                 GLuint *program_id)
{
  GLuint vertex_shader = 0;
  GLuint fragment_shader = 0;

  *program_id = glCreateProgram();
  if (vert_file)
      vertex_shader = attach_shader(vert_file, GL_VERTEX_SHADER, *program_id);
  if (frag_file)
      fragment_shader = attach_shader(frag_file, GL_FRAGMENT_SHADER, *program_id);

  glLinkProgram(*program_id);

  if (vertex_shader)
      destroy_shader(vertex_shader, *program_id);
  if (fragment_shader)
      destroy_shader(fragment_shader, *program_id);

  printProgramInfoLog(*program_id);
}
