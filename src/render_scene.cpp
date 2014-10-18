#include <GL/glew.h>

#include "render_scene.hpp"

scene::scene(GLuint program_id, float aspect_ratio, GLFWwindow *window)
    : program_id_(program_id),
      proj_mat_(glm::perspective(glm::radians(56.25f), aspect_ratio, 0.1f, 10000.0f)),
      view_mat_(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -70, -100.0), glm::vec3(0, 1, 0))),
      translation_(glm::vec3(0, -100.0f, -100.0f)),
      rotation_(glm::vec3(0, 0, 0)),
      window_(window)
{
    (void) padding_;
}

void
scene::update()
{
    const devices_state &device = devices_state::get_instance(window_);
    update_rotation(device);
    update_position(device);
    update_light_source();
    set_model_view_matrix();
}

void
scene::update_light_source()
{
  GLint light_dir_idx = glGetUniformLocation(program_id_, "light_dir");

  glm::vec3 light_dir(1, 0, 0.8);

  // When world moves, light moves with it, therefore we multiply it by a normal
  // matrix
  glUniform3fv(light_dir_idx, 1, glm::value_ptr(glm::normalize(normal_mat_ * light_dir)));
}

void
scene::set_material_color()
{
  GLint light_dir_idx = glGetUniformLocation(program_id_, "material.specular");

  glm::vec3 specular(1, 0, 0.8);

  glUniform3fv(light_dir_idx, 1, glm::value_ptr(specular));
}

void
scene::set_model_view_matrix()
{
  GLint projMatIdx = glGetUniformLocation(program_id_, "projMat");
  GLint model_view_idx = glGetUniformLocation(program_id_, "model_view_mat");
  GLint normal_mat_idx = glGetUniformLocation(program_id_, "normal_mat");

  glm::mat4 modelMat; // identity
  modelMat = glm::rotate(modelMat, rotation_.x, glm::vec3(1, 0, 0));
  modelMat = glm::rotate(modelMat, rotation_.y, glm::vec3(0, 1, 0));
  modelMat = glm::translate(modelMat, translation_);

  glm::mat4 model_view_mat = view_mat_ * modelMat;
  normal_mat_ = glm::transpose(glm::inverse(glm::mat3(model_view_mat)));

  glUniformMatrix4fv(projMatIdx, 1, GL_FALSE, glm::value_ptr(proj_mat_));
  glUniformMatrix4fv(model_view_idx, 1, GL_FALSE, glm::value_ptr(model_view_mat));
  glUniformMatrix3fv(normal_mat_idx, 1, GL_FALSE, glm::value_ptr(normal_mat_));
}

void
scene::render_arrays(GLuint vao_id, int nb_elt)
{
  glBindVertexArray(vao_id);
  glDrawArrays(GL_TRIANGLES, 0, nb_elt);
  glBindVertexArray(0);
}

void
scene::render_elements(GLuint vao_id, int nb_elt)
{
  glBindVertexArray(vao_id);
  glDrawElements(GL_TRIANGLES, nb_elt, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void
scene::update_rotation(const devices_state &device)
{
    double xpos, ypos;
    device.get_mouse_movement(&xpos, &ypos);

    static double old_x = 0;
    static double old_y = 0;
    int height, width;
    glfwGetWindowSize(window_, &width, &height);
    glfwGetCursorPos(window_, &xpos, &ypos);
    // When mouse moves along y axis, the scene is rotated along x axis.
    rotation_.x += static_cast<float>((ypos - old_y) / (height));
    // When mouse moves along x axis, the scene is rotated along y axis.
    rotation_.y += static_cast<float>((xpos - old_x) / (width));
    old_x = xpos;
    old_y = ypos;

}

void
scene::update_position(const devices_state &device)
{
    const float pos_incr = 10.0f;
    const float rot_incr = 0.07f;
    const float pi = 3.141592653589f;

    if (device.key_state.left_pressed) {
        translation_.x += glm::sin(pi / 2 - rotation_.y) * pos_incr;
        translation_.z += glm::cos(pi / 2 - rotation_.y) * pos_incr;
    }
    if (device.key_state.right_pressed) {
        translation_.x -= glm::sin(pi / 2 - rotation_.y) * pos_incr;
        translation_.z -= glm::cos(pi / 2 - rotation_.y) * pos_incr;
    }
    if (device.key_state.up_pressed) {
        translation_.z += glm::cos(-rotation_.y) * pos_incr;
        translation_.x += glm::sin(-rotation_.y) * pos_incr;
    }
    if (device.key_state.down_pressed) {
        translation_.z -= glm::cos(-rotation_.y) * pos_incr;
        translation_.x -= glm::sin(-rotation_.y) * pos_incr;
    }
    if (device.key_state.w_pressed) {
        rotation_.x -= rot_incr;
    }
    if (device.key_state.s_pressed) {
        rotation_.x += rot_incr;
    }
    if (device.key_state.a_pressed) {
        rotation_.y -= rot_incr;
    }
    if (device.key_state.d_pressed) {
        rotation_.y += rot_incr;
    }
}
