#include "light.hpp"
#include <glm/gtc/matrix_transform.hpp>

dir_light* light_dir_default_new()
{
    dir_light* res = new dir_light;
    glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
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
    glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 pos(0.0f, 100.0f, 100.2f, 1.0f);
    res->set_ambient(ambient);
    res->set_diffuse(diffuse);
    res->set_specular(specular);
    res->set_position(pos);
    return res;
}

cone_light* light_cone_default_new()
{
    cone_light* res = new cone_light;
    glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 pos(0.0f, 100.0f, 100.2f, 1.0f);
    res->set_ambient(ambient);
    res->set_diffuse(diffuse);
    res->set_specular(specular);
    res->set_position(pos);
    return res;
}
