#ifndef UTILITY_HPP
# define UTILITY_HPP

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

#endif //UTILITY_HPP
