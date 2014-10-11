#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <stdio.h>
#include <stdlib.h>
#include <array>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "textfile.h"



GLuint p;
GLuint vaoID[2];

GLuint modelMatIdx;
GLuint viewMatIdx;
GLuint projMatIdx;


void renderScene(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 modelMat; // identity
  glm::mat4 projMat = glm::perspective(56.25f, 640.0f/480.0f, 0.1f, 100.0f);
  glm::mat4 viewMat = glm::lookAt(glm::vec3(0, 6, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

  glUniformMatrix4fv(projMatIdx, 1, GL_FALSE, glm::value_ptr(projMat));
  glUniformMatrix4fv(viewMatIdx, 1, GL_FALSE, glm::value_ptr(viewMat));
  glUniformMatrix4fv(modelMatIdx, 1, GL_FALSE, glm::value_ptr(modelMat));

  glBindVertexArray(vaoID[0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


void printShaderInfoLog(GLuint obj)
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

void printProgramInfoLog(GLuint obj)
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


void BindArrays()
{
  glGenVertexArrays(1, vaoID);

  glBindVertexArray(vaoID[0]);

  GLuint vertBufferID, indexBufferID;
  glGenBuffers(1, &vertBufferID);
  glGenBuffers(1, &indexBufferID);

  struct Vec2 { float x, y; };
  struct Vec3 { float x, y, z; };
  struct Vert { Vec3 pos; Vec2 tex; };

  std::array<Vert, 8> cubeVerts = {{
    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } }, { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } },
        { {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } }, { {  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
        { { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f } }, { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f } },
        { { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f } }, { { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f } }
  }};

  std::array<unsigned int, 36> cubeIdxs = {{
    0, 2, 1, 0, 3, 2, // Right
        4, 5, 6, 4, 6, 7, // Left
        0, 7, 3, 0, 4, 7, // Top
        1, 6, 2, 1, 5, 6, // Bottom
        0, 5, 1, 0, 4, 5, // Front
        3, 7, 6, 3, 6, 2  // Back
  }};

  // Vertex buffer
  GLuint pos_idx = glGetAttribLocation(p, "in_position");
  GLuint uv_idx = glGetAttribLocation(p, "in_uv");
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vert) * cubeVerts.size(), cubeVerts.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(pos_idx); // Matches layout (location = 0)
  glVertexAttribPointer(pos_idx, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), 0);
  glEnableVertexAttribArray(uv_idx); // Matches layout (location = 1)
  glVertexAttribPointer(uv_idx, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (GLvoid*)sizeof(Vec3));

  // Index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * cubeIdxs.size(), cubeIdxs.data(), GL_STATIC_DRAW);
  glBindVertexArray(0);
}

void loadShaders()
{
  char *vs = NULL,*fs = NULL;

  GLuint v = glCreateShader(GL_VERTEX_SHADER);
  GLuint f = glCreateShader(GL_FRAGMENT_SHADER);

  const char *vert_file = "src/shaders/height.vert";
  const char *frag_file = "src/shaders/height.frag";
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

  p = glCreateProgram();
  glAttachShader(p,v);
  glAttachShader(p,f);

  glLinkProgram(p);
  printProgramInfoLog(p);

  glUseProgram(p);


  modelMatIdx = glGetUniformLocation(p, "modelMat");
  viewMatIdx = glGetUniformLocation(p, "viewMat");
  projMatIdx = glGetUniformLocation(p, "projMat");

  BindArrays();
}

void enableEnv()
{
  glEnable(GL_DEPTH_TEST);
}

void loadTextures()
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
  GLint loc0 = glGetUniformLocation(p, "texture0");
  glUniform1i(loc0, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  image = SOIL_load_image("canion.jpg", &width, &height, &channel, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB ,GL_UNSIGNED_BYTE, image);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  GLint loc1 = glGetUniformLocation(p, "texture1");
  printf("%d, %d\n", loc0, loc1);
  glUniform1i(loc1, 1);
}

int main(void)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* We are using core version, so glewExperimental needs to be set */
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK)
    exit(1); // or handle the error in a nicer way

  enableEnv();
  loadShaders();
  loadTextures();

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
    {
      /* Render here */

      /* Swap front and back buffers */
      renderScene();
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

  glfwTerminate();
  return 0;
}
