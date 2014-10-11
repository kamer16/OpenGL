#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <stdio.h>
#include <stdlib.h>
#include <array>

GLuint texture[2];
float vertices[] = { 0, 0,  0, 3,  3, 3,  3, 0 };
float colors[] = { 1.0f, 0, 0,  1, 0, 0,  1, 0, 0 };
float tex_coord[] = { 1.0f, 0,  1, 1,  0, 1,  0, 0, 0 };
float fog[] = { 1.0f, 0.5f,  0.2f, 1,  0.4f, 1,  0, 0.2f, 0.6f };

void renderScene(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(0.0,0.0,5.0,
            0.0,0.0,-1.0,
            0.0f,1.0f,0.0f);

  glBegin(GL_QUADS);
    glMultiTexCoord2f(GL_TEXTURE0, 0, 0);
    glMultiTexCoord2f(GL_TEXTURE1, 0, 0);
    glVertex3f(-1, 0, 0);

    glMultiTexCoord2f(GL_TEXTURE0, 1, 0);
    glMultiTexCoord2f(GL_TEXTURE1, 1, 0);
    glVertex3f(0, 0, 0);

    glMultiTexCoord2f(GL_TEXTURE0, 1, 1);
    glMultiTexCoord2f(GL_TEXTURE1, 1, 1);
    glVertex3f(0, 1, 0);

    glMultiTexCoord2f(GL_TEXTURE0, 0, 1);
    glMultiTexCoord2f(GL_TEXTURE1, 0, 1);
    glVertex3f(-1, 1, 0);
  glEnd();


  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_FOG_COORDINATE_ARRAY);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisable(GL_FOG_COORDINATE_ARRAY);

}

void enableEnv()
{
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_FOG);
  glFogi (GL_FOG_MODE, GL_EXP2);
  GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};
  GLfloat density = 0.01f;
  glFogf (GL_FOG_DENSITY, density);
  glFogfv (GL_FOG_COLOR, fogColor);
  //glHint (GL_FOG_HINT, GL_NICEST);
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

void loadTexture()
{
  int width;
  int height;
  unsigned char *image;

  glGenTextures(2, texture);
  {
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D); // Necessary when no shaders
    image = SOIL_load_image("tree.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    SOIL_free_image_data(image);
  }

  {
    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D); // Necessary when no shaders
    image = SOIL_load_image("tree.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    SOIL_free_image_data(image);
  }
}

void loadVBO()
{
  glVertexPointer(2, GL_FLOAT, 0, vertices);
  glColorPointer(3, GL_FLOAT, 0, colors);

  glClientActiveTexture(GL_TEXTURE1);
  glTexCoordPointer(2, GL_FLOAT, 0, tex_coord);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glClientActiveTexture(GL_TEXTURE0);
  glTexCoordPointer(2, GL_FLOAT, 0, tex_coord);

  glFogCoordPointer(GL_FLOAT, 0, fog);
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
  loadTexture();
  loadVBO();

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
