#ifndef UTILITY_HPP
# define UTILITY_HPP

# include <glm/glm.hpp>

namespace utility
{
    struct color_vert
    {
        glm::vec3 pos;
        glm::vec3 color;
    };
}

void print(glm::mat4 mat);
void print(glm::mat3 mat);
void print(glm::vec4 mat);
void print(glm::vec3 mat);
void print(glm::vec2 mat);

#endif //UTILITY_HPP
