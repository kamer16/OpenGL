#include "light.hpp"
#include <glm/gtc/matrix_transform.hpp>

light* default_dir_light_new()
{
    light* res = new light;
    res->set_ambient({ 0.0f, 0.0f, 0.0f, 1.0f });
    res->set_diffuse({ 1.0f, 1.0f, 1.0f, 1.0f });
    res->set_specular({ 1.0f, 1.0f, 1.0f, 1.0f });
    glm::vec4 pos(0.0f, 1.0f, 0.2f, 0.0f);
    res->set_position(glm::normalize(pos));
    return res;
}

light* default_pos_light_new()
{
    light* res = new light;
    res->set_ambient({ 0.0f, 0.0f, 0.0f, 1.0f });
    res->set_diffuse({ 1.0f, 1.0f, 1.0f, 1.0f });
    res->set_specular({ 1.0f, 1.0f, 1.0f, 1.0f });
    glm::vec4 pos(0.0f, 1.0f, 0.2f, 1.0f);
    res->set_position(glm::normalize(pos));
    return res;
}
