#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp" // loadShaders()
#include "texture.hpp" // loadTextures()
#include "vbo.hpp" // BindArrays()
#include "render_scene.hpp" // renderScene()
#include "polygon.hpp" // polygons

void enableEnv()
{
  glEnable(GL_DEPTH_TEST);
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
  glGetError();

  if (err != GLEW_OK)
    return -1; // or handle the error in a nicer way

  enableEnv();
  GLuint program_ids[2];
  loadShaders("src/shaders/height.vert", "src/shaders/height.frag",
              &program_ids[0]);
  loadShaders("src/shaders/color.vert", "src/shaders/color.frag",
              &program_ids[1]);

  GLuint vaoID[16];
  BindArrays(program_ids[0], vaoID);
  loadTextures(program_ids[0]);
  polygon *coord = coordinate_polygon_new(program_ids[1]);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
    {
      /* Render here */

      /* Swap front and back buffers */
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram(program_ids[0]);
      renderScene(program_ids[0], vaoID);
      glUseProgram(program_ids[1]);
      coord->draw();
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

  glfwTerminate();
  return 0;
}
