#include <stdlib.h> // malloc, free
#include <stdio.h> // printf

#include "textfile.hpp"
#include "shader.hpp"

static void printShaderInfoLog(GLuint obj)
{
  int infologLength = 0;
  int charsWritten  = 0;
  char *infoLog;

  glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

  if (infologLength > 0)
    {
      infoLog = (char *)malloc(infologLength);
      glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
      printf("%s\n",infoLog);
      free(infoLog);
    }
}

static void printProgramInfoLog(GLuint obj)
{
  int infologLength = 0;
  int charsWritten  = 0;
  char *infoLog;

  glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

  if (infologLength > 0)
    {
      infoLog = (char *)malloc(infologLength);
      glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
      printf("%s\n",infoLog);
      free(infoLog);
    }
}


void loadShaders(const char *vert_file, const char *frag_file,
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

  free(vs);free(fs);

  glCompileShader(v);
  glCompileShader(f);

  printShaderInfoLog(v);
  printShaderInfoLog(f);

  *program_id = glCreateProgram();
  glAttachShader(*program_id,v);
  glAttachShader(*program_id,f);

  glLinkProgram(*program_id);

  glDetachShader(*program_id, v);
  glDetachShader(*program_id, f);
  glDeleteShader(v);
  glDeleteShader(f);

  printProgramInfoLog(*program_id);

  glUseProgram(*program_id);
}
