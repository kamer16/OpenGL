#ifndef UTILITY_HPP
# define UTILITY_HPP

namespace utility
{

struct vec2 { float x, y; };
struct vec3 { float x, y, z; };
struct vec4 { float x, y, z, w; };
struct vert { vec3 pos; vec2 tex; };
struct color_vert { vec3 pos; vec3 color; };

}

#endif //UTILITY_HPP
