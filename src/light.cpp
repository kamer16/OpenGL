#include "light.hpp"
#include <glm/gtc/matrix_transform.hpp>

light* light_dir_default_new()
{
    light* res = new light;
    glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 pos(0.0f, 1.0f, 0.2f, 0.0f);
    res->set_ambient(ambient);
    res->set_diffuse(diffuse);
    res->set_specular(specular);
    res->set_position(glm::normalize(pos));
    return res;
}

light* light_pos_default_new()
{
    light* res = new light;
    glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 pos(0.0f, 1.0f, 0.2f, 1.0f);
    res->set_ambient(ambient);
    res->set_diffuse(diffuse);
    res->set_specular(specular);
    res->set_position(glm::normalize(pos));
    return res;
}
