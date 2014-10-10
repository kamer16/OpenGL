#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <array>


void renderScene(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(0.0,0.0,5.0,
            0.0,0.0,-1.0,
            0.0f,1.0f,0.0f);
  glBegin(GL_QUADS);
    glColor3f(1, 0, 0);

    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);

    glTexCoord2f(1, 0);
    glVertex3f(1, 0, 0);

    glTexCoord2f(1, 1);
    glVertex3f(1, 1, 0);

    glTexCoord2f(0, 1);
    glVertex3f(0, 1, 0);
  glEnd();
}

void enableEnv()
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
}

void setView()
{
  // Reset the coordinate system before modifying
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set the viewport to be the entire window
  glViewport(0, 0, 640, 480);

  // Set the correct perspective.
  gluPerspective(45, 640.0 / 480.0,1 ,1000);
  glMatrixMode(GL_MODELVIEW);
}

int main(void)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  GLenum err = glewInit();
  if (err != GLEW_OK)
    exit(1); // or handle the error in a nicer way

  enableEnv();
  setView();

  glMatrixMode(GL_MODELVIEW);
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
