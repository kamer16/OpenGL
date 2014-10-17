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

#include "obj_loader.hpp"

#include <iostream>

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
    static double old_x = x;
    static double old_y = y;
    int height, width;
    glfwGetWindowSize(w, &width, &height);
    // Allow a rotation of up to 45 degrees left and right.
    // Make center of screen the initial position
    g_angle_y += (x - old_x) / (width);
    g_angle_x += (y - old_y) / (height);
    old_x = x;
    old_y = y;
}

static struct s_keyboard_state
{
    char up_pressed;
    char down_pressed;
    char right_pressed;
    char left_pressed;
    char w_pressed;
    char a_pressed;
    char s_pressed;
    char d_pressed;
} g_key_state;

static void update_position()
{
    const float pos_incr = 10.0f;
    const float rot_incr = 0.07f;
    const float pi = 3.141592653589f;
    // Left arraw
    if (g_key_state.left_pressed) {
        g_x += glm::sin(pi / 2 - g_angle_y) * pos_incr;
        g_z += glm::cos(pi / 2 - g_angle_y) * pos_incr;
    }
    // Right arraw
    if (g_key_state.right_pressed) {
        g_x -= glm::sin(pi / 2 - g_angle_y) * pos_incr;
        g_z -= glm::cos(pi / 2 - g_angle_y) * pos_incr;
    }
    // Up arraw
    if (g_key_state.up_pressed) {
        g_z += glm::cos(-g_angle_y) * pos_incr;
        g_x += glm::sin(-g_angle_y) * pos_incr;
    }
    // Down arraw
    if (g_key_state.down_pressed) {
        g_z -= glm::cos(-g_angle_y) * pos_incr;
        g_x -= glm::sin(-g_angle_y) * pos_incr;
    }
    // W key
    if (g_key_state.w_pressed) {
        g_angle_x -= rot_incr;
    }
    // S key
    if (g_key_state.s_pressed) {
        g_angle_x += rot_incr;
    }
    // A key
    if (g_key_state.a_pressed) {
        g_angle_y -= rot_incr;
    }
    // D key
    if (g_key_state.d_pressed) {
        g_angle_y += rot_incr;
    }
}

void handle_keyboard(GLFWwindow *w, int key, int scancode, int action, int mods)
{
    (void) w;
    (void) key;
    (void) mods;
    if (action != GLFW_PRESS && action != GLFW_RELEASE)
        return;
    // Left arraw
    if (scancode == 113) {
        g_key_state.left_pressed = action == GLFW_PRESS;
    }
    // Right arraw
    else if (scancode == 114) {
        g_key_state.right_pressed = action == GLFW_PRESS;
    }
    // Up arraw
    else if (scancode == 111) {
        g_key_state.up_pressed = action == GLFW_PRESS;
    }
    // Down arraw
    else if (scancode == 116) {
        g_key_state.down_pressed = action == GLFW_PRESS;
    }
    // W key
    else if (scancode == 25) {
        g_key_state.w_pressed = action == GLFW_PRESS;
    }
    // S key
    else if (scancode == 39) {
        g_key_state.s_pressed = action == GLFW_PRESS;
    }
    // A key
    else if (scancode == 38) {
        g_key_state.a_pressed = action == GLFW_PRESS;
    }
    // D key
    else if (scancode == 40) {
        g_key_state.d_pressed = action == GLFW_PRESS;
    }
}

int main(int argc, char *argv[])
{
  (void) argv;
  g_angle_x =  g_angle_y = g_x = 0;
  g_z = -100.0f;
  g_y = -100.5f;
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  GLFWmonitor *monitor = argc > 1 ? glfwGetPrimaryMonitor() : NULL;
  int width = 880;
  int height = 520;
  float aspect_ration = static_cast<float>(width) / static_cast<float>(height);
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
  polygon *coord = polygon::coordinate_new(program_ids[1]);
  polygon *ground = polygon::quad_xz_new(program_ids[1]);
  glfwSetKeyCallback(window, handle_keyboard);
  glfwSetCursorPosCallback(window, handle_mouse_position);

  std::vector<utility::vec3> vertices;
  std::vector<utility::vec3> normals;
  std::vector<utility::vec2> text_coords;
  load_obj("data/teapot/teapot.obj", vertices, normals, text_coords);
  GLuint mesh_vao_id;
  bind_object(program_ids[0], &mesh_vao_id, vertices, normals, text_coords);
  if (monitor)
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  else // TODO somehow not working, Bug in GLFW ??
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
    {
      /* Render here */
        update_position();

      /* Swap front and back buffers */
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram(program_ids[0]);
      set_model_view_matrix(program_ids[0], aspect_ration);
      render_elements(vaoID, 36);
      render_arrays(&mesh_vao_id, static_cast<int>(vertices.size()));
      glUseProgram(program_ids[1]);
      set_model_view_matrix(program_ids[1], aspect_ration);
      coord->draw();
      ground->draw();
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

  glfwTerminate();
  return 0;
}
