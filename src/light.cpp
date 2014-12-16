#include "light.hpp"

# define GLM_FORCE_RADIANS

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
# include <glm/gtc/type_ptr.hpp> /* value_ptr */

dir_light* light_dir_default_new()
{
    dir_light* res = new dir_light;
    glm::vec4 light_color(0.0f, 0.0f, 1.0, 1.0f);
    glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 diffuse = glm::vec4(light_color);
    glm::vec4 specular = glm::vec4(light_color);
    glm::vec4 pos(0.0f, 1.0f, 0.1f, 0.0f);
    res->set_ambient(ambient);
    res->set_diffuse(diffuse);
    res->set_specular(specular);
    res->set_position((pos));
    return res;
}

pos_light* light_pos_default_new()
{
    pos_light* res = new pos_light;
    glm::vec4 light_color(0.0f, 1.0f, 0.0, 1.0f);
    glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 diffuse = glm::vec4(light_color);
    glm::vec4 specular = glm::vec4(light_color);
    glm::vec4 pos(80.0f, 10.0f, 0.0f, 1.0f);
    res->set_ambient(ambient);
    res->set_diffuse(diffuse);
    res->set_specular(specular);
    res->set_position(pos);
    return res;
}

spot_light* light_spot_default_new()
{
    spot_light* res = new spot_light;
    glm::vec4 light_color(1.0f, 1.0f, 1.0, 1.0f);
    glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 diffuse = glm::vec4(light_color);
    glm::vec4 specular = glm::vec4(light_color);
    glm::vec4 pos(0.0f, 155.0f, 180.0f, 1.0f);
    glm::vec3 dir(0.0f, -0.8f, -1.0f);
    res->set_ambient(ambient);
    res->set_diffuse(diffuse);
    res->set_specular(specular);
    res->set_position(pos);
    res->set_spot_dir(dir);
    return res;
}

float
pos_light::get_scale()
{
    float c = 0;
    for (int i = 0; i < 3; ++i)
        c = diffuse_[i] < c ? c : diffuse_[i];
    float dist = -linear_att_;
    dist += sqrtf(linear_att_ * linear_att_ - 4 * quadratic_att_ *
                  (const_att_ - 256 * c));
    dist /=  2 * quadratic_att_;
    return dist;
}

glm::mat4
spot_light::get_bias_cam_to_light_mvp(const glm::mat4& cam_view_mat)
{
    const glm::mat4 bias_mat(0.5f, 0, 0, 0.0f,
                             0, 0.5f, 0, 0.0f,
                             0, 0, 0.5f, 0.0f,
                             0.5f, 0.5f, 0.5f, 1.0f);
    return bias_mat * get_vp() * glm::inverse(cam_view_mat);
}

glm::mat4
spot_light::get_vp()
{
    auto view_mat = glm::lookAt(glm::vec3(get_position()),
                                glm::vec3(get_position()) +
                                get_spot_dir(),
                                glm::vec3(0, 1, 0));
    auto proj_mat = glm::perspective(glm::radians(80.0f), 880.0f / 520.0f,
                                     1.f, 1 * 500.0f);
    return proj_mat * view_mat;
}

glm::mat4
dir_light::get_bias_cam_to_light_mvp(const glm::mat4& cam_view_mat)
{
    const glm::mat4 bias_mat(0.5f, 0, 0, 0.0f,
                             0, 0.5f, 0, 0.0f,
                             0, 0, 0.5f, 0.0f,
                             0.5f, 0.5f, 0.5f, 1.0f);
    return bias_mat * get_vp() * glm::inverse(cam_view_mat);
}

glm::mat4
dir_light::get_vp()
{
    auto view_mat = glm::lookAt(glm::vec3(0), -glm::vec3(get_position()),
                                glm::vec3(0, 1, 0));
    auto proj_mat = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, -500.0f, 500.0f);
    return proj_mat * view_mat;
}
