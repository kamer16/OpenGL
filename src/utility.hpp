#ifndef UTILITY_HPP
# define UTILITY_HPP

# include <glm/glm.hpp>

namespace utility
{

struct vec2
{
    using value_type = float;
    float x, y;
};

struct vec3
{
    using value_type = float;
    float x, y, z;
};

struct vec4
{
    using value_type = float;
    float x, y, z, w;
};

struct vert
{
    vec3 pos;
    vec2 tex;
};

struct color_vert
{
    vec3 pos;
    vec3 color;
};

}

void print(glm::mat4 mat);
void print(glm::mat3 mat);
void print(glm::vec4 mat);
void print(glm::vec3 mat);
void print(glm::vec2 mat);

#endif //UTILITY_HPP
