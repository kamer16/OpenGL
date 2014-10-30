#include <stdio.h> // printf

#include "textfile.hpp"
#include "shader.hpp"

static void printShaderInfoLog(GLuint obj, const char *file)
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


void load_shaders(const char *vert_file, const char *frag_file,
                 GLuint *program_id)
{
  char *vs = NULL,*fs = NULL;

  GLuint v = glCreateShader(GL_VERTEX_SHADER);
  GLuint f = glCreateShader(GL_FRAGMENT_SHADER);

  vs = textFileRead(vert_file);
  fs = textFileRead(frag_file);

  const char * vv = vs;
  const char * ff = fs;

  glShaderSource(v, 1, &vv,NULL);
  glShaderSource(f, 1, &ff,NULL);

  delete[] vs;
  delete[] fs;

  glCompileShader(v);
  glCompileShader(f);

  printShaderInfoLog(v, "Vertex");
  printShaderInfoLog(f, "Fragment");

  *program_id = glCreateProgram();
  glAttachShader(*program_id,v);
  glAttachShader(*program_id,f);

  glLinkProgram(*program_id);

  glDetachShader(*program_id, v);
  glDetachShader(*program_id, f);
  glDeleteShader(v);
  glDeleteShader(f);

  printProgramInfoLog(*program_id);
}
