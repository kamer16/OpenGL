#include "camera.hpp"

#include "utility.hpp"

camera::camera(float aspect_ratio)
    : proj_mat_(glm::perspective(glm::radians(56.25f), aspect_ratio, 0.1f,
                                 10000.0f)),
      view_mat_(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -70, -100.0),
                            glm::vec3(0, 1, 0))),
      model_mat_(glm::translate(glm::mat4(), glm::vec3(0, -100.0f, -100.0f)))
{
}

const glm::mat4&
camera::get_model_mat()
{
    return model_mat_;
}

const glm::mat4&
camera::get_proj_mat()
{
    return proj_mat_;
}

#include <iostream>
const glm::mat4&
camera::get_view_mat()
{
    position_ = -translation;
    //glm::vec4 center = glm::vec4(position_.x, position_.y, position_.z - 1, 1);
    //glm::mat4 rotation = glm::translate(glm::mat4(),glm::vec3(position_.x,
    //                                                           position_.y,
    //                                                           position_.z));

    //glm::mat4 x_rotation = glm::rotate(glm::mat4(), -rotation_.x, glm::vec3(1, 0, 0));
    //glm::mat4 y_rotation = glm::rotate(glm::mat4(), -rotation_.y, glm::vec3(0, 1, 0));

    glm::vec3 center = glm::vec3(0, glm::sin(-rotation_.x), -glm::cos(-rotation_.x) -
                                 glm::sin(rotation_.y));
    center += glm::vec3(glm::sin(rotation_.y), 0, -glm::cos(rotation_.y));

    //rotation = glm::translate(rotation, glm::vec3(0, 0, -1));

    //glm::vec3 center_pos = glm::vec3(x_rotation * glm::vec4(1, 1, 1, 1));
    view_mat_ = glm::lookAt(position_, position_ + center , glm::vec3(0, 1, 0)); 

    print(center);
    print(-rotation_);
    //print(position_);
    //print(rotation_);
    //print_vec3(center_pos);
    //print_mat4(rotation);
    //view_mat_ = glm::lookAt(position_, glm::vec3(position_.x, position_.y,
    //                                             position_.z - 1), glm::vec3(0, 1, 0)); 
    return view_mat_;
}

void
camera::update(const devices_state &device)
{
    update_rotation(device);
    update_position(device);
}

void
camera::update_rotation(const devices_state &device)
{
    double xpos, ypos;
    device.get_mouse_movement(&xpos, &ypos);
    rotation_.x += static_cast<float>(ypos);
    rotation_.y += static_cast<float>(xpos);
    // When mouse moves along y axis, the scene is rotated along x axis.
    model_mat_ = glm::rotate(model_mat_, static_cast<float>(xpos),
                             glm::vec3(0, 1, 0));
    // When mouse moves along x axis, the scene is rotated along y axis.
    model_mat_ = glm::rotate(model_mat_, static_cast<float>(ypos),
                             glm::vec3(1, 0, 0));
}

void
camera::update_position(const devices_state &device)
{
    const float pos_incr = 10.0f;
    const float rot_incr = 0.07f;
    const float pi = 3.141592653589f;
    glm::vec2 rotation;

    if (device.key_state.left_pressed) {
        translation.x += glm::sin(pi / 2 - rotation_.y) * pos_incr;
        translation.z += glm::cos(pi / 2 - rotation_.y) * pos_incr;
    }
    if (device.key_state.right_pressed) {
        translation.x -= glm::sin(pi / 2 - rotation_.y) * pos_incr;
        translation.z -= glm::cos(pi / 2 - rotation_.y) * pos_incr;
    }
    if (device.key_state.up_pressed) {
        translation.z += glm::cos(-rotation_.y) * pos_incr;
        translation.x += glm::sin(-rotation_.y) * pos_incr;
    }
    if (device.key_state.down_pressed) {
        translation.z -= glm::cos(-rotation_.y) * pos_incr;
        translation.x -= glm::sin(-rotation_.y) * pos_incr;
    }
    if (device.key_state.w_pressed) {
        rotation_.x -= rot_incr;
        rotation.x -= rot_incr;
    }
    if (device.key_state.s_pressed) {
        rotation_.x += rot_incr;
        rotation.x += rot_incr;
    }
    if (device.key_state.a_pressed) {
        rotation_.y -= rot_incr;
        rotation.y -= rot_incr;
    }
    if (device.key_state.d_pressed) {
        rotation_.y += rot_incr;
        rotation.y += rot_incr;
    }
    model_mat_ = glm::translate(model_mat_, translation);
    model_mat_ = glm::rotate(model_mat_, rotation.x, glm::vec3(1, 0, 0));
    model_mat_ = glm::rotate(model_mat_, rotation.y, glm::vec3(0, 1, 0));
}

