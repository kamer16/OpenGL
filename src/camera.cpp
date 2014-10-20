#include "camera.hpp"

#include "utility.hpp"

camera::camera(float aspect_ratio)
    : position_(0, 100, 100.0),
      rotation_(-0.4, -3.14),
      aspect_ratio_(aspect_ratio),
      fov_(45.0f)
{
}

const glm::mat4&
camera::get_proj_mat()
{
    proj_mat_ = glm::perspective(glm::radians(fov_), aspect_ratio_,
                                 0.1f, 10000.0f);
    return proj_mat_;
}

#include <iostream>
const glm::mat4&
camera::get_view_mat()
{
    view_mat_ = glm::lookAt(position_, position_ + forward_,
                            glm::vec3(0, 1, 0)); 
    return view_mat_;
}

void
camera::update(const devices_state &device)
{
    update_rotation(device);
    update_position(device);
    // Spherical coordinates to cartesian coordinates formula
    forward_ = glm::vec3(glm::cos(rotation_.x) * glm::sin(rotation_.y),
                         glm::sin(rotation_.x),
                         glm::cos(rotation_.x) * glm::cos(rotation_.y));
    // Always on horizontal i-e y == 0
    // direction is the one of y_axis rotation - pi/2.
    right_ = glm::vec3(glm::sin(rotation_.y - 3.14f / 2.0f),
                       0,
                       glm::cos(rotation_.y - 3.14f / 2.0f));
    up_ = glm::cross(right_, forward_);
}

void
camera::update_rotation(const devices_state &device)
{
    double xpos, ypos;
    device.get_mouse_movement(&xpos, &ypos);
    // When mouse moves along y axis, the scene is rotated along x axis.
    rotation_.x -= static_cast<float>(ypos);
    // When mouse moves along x axis, the scene is rotated along y axis.
    rotation_.y -= static_cast<float>(xpos);
}

void
camera::update_position(const devices_state &device)
{
    const float pos_incr = 10.0f;
    const float rot_incr = 0.07f;

    if (device.key_state.left_pressed) {
        position_ -= right_ * pos_incr;
    }
    if (device.key_state.right_pressed) {
        position_ += right_ * pos_incr;
    }
    if (device.key_state.up_pressed) {
        position_ += forward_ * pos_incr;
    }
    if (device.key_state.down_pressed) {
        position_ -= forward_ * pos_incr;
    }
    if (device.key_state.w_pressed) {
        rotation_.x += rot_incr;
    }
    if (device.key_state.s_pressed) {
        rotation_.x -= rot_incr;
    }
    if (device.key_state.a_pressed) {
        rotation_.y += rot_incr;
    }
    if (device.key_state.d_pressed) {
        rotation_.y -= rot_incr;
    }
}

