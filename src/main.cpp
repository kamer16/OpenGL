#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
#include <glm/gtc/type_ptr.hpp> /* value_ptr */

#include "shader.hpp" // loadShaders()
#include "texture.hpp" // loadTextures()
#include "vbo.hpp" // BindArrays()
#include "render_scene.hpp" // renderScene()
#include "polygon.hpp" // polygons

void enableEnv();
void handle_mouse_position(GLFWwindow *w, double x, double y);
void handle_keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);

void enableEnv()
{
  glEnable(GL_DEPTH_TEST);
}

extern double g_x, g_y, g_z, g_angle_x, g_angle_y;
double g_x, g_y, g_z, g_angle_x, g_angle_y;

void handle_mouse_position(GLFWwindow *w, double x, double y)
{
    const double pi = 3.14159265359;
    int height, width;
    glfwGetWindowSize(w, &width, &height);
    // Allow a rotation of up to 45 degrees left and right.
    // Make center of screen the initial position
    g_angle_x = (x - width / 2) / (width / 2) * (pi / 2);
    g_angle_y = (y - height / 2) / (height / 2) * (pi / 2);
}

void handle_keyboard(GLFWwindow *w, int key, int scancode, int action, int mods)
{
    (void) w;
    (void) key;
    (void) action;
    (void) mods;
    const float incr = 1.0;
    if (action !=  GLFW_PRESS)
        return;
    // Left arraw
    if (scancode == 113) {
        g_x -= incr;
    }
    // Up arraw
    else if (scancode == 111) {
        g_z += 1;
        g_x += g_angle_x;
        printf("moving: (%f, %f)\n", g_angle_x, 1.0f);
    }
    // Down arraw
    else if (scancode == 116) {
        g_z-= 1;
    }
    // Right arraw
    else if (scancode == 114) {
        g_x += incr;
    }
    else if (scancode == 25) {
        g_y += incr;
    }
    else if (scancode == 39) {
        g_y -= incr;
    }
}

int main(void)
{
  g_angle_x =  g_angle_y = g_x = g_y = g_z = 0.0f;
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  GLFWmonitor *monitor = NULL;
  int width = 1080;
  int height = 768;
  float aspect_ration = (float) width / (float) height;
  window = glfwCreateWindow(width, height, "Hello World", monitor, NULL);
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
  glfwSetKeyCallback(window, handle_keyboard);
  glfwSetCursorPosCallback(window, handle_mouse_position);
  if (monitor)
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  else // TODO somehow not working, Bug in GLFW ??
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
    {
      /* Render here */

      /* Swap front and back buffers */
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram(program_ids[0]);
      set_model_view_matrix(program_ids[0], aspect_ration);
      renderScene(vaoID);
      glUseProgram(program_ids[1]);
      set_model_view_matrix(program_ids[1], aspect_ration);
      coord->draw();
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

  glfwTerminate();
  return 0;
}
