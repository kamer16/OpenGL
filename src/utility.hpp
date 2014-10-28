#ifndef UTILITY_HPP
# define UTILITY_HPP

# include <glm/glm.hpp>
# include <sstream>
# include <iostream>
# include <string>
# include <tuple>

class hash_ptr
{
public:
    size_t operator() (const std::tuple<size_t, size_t, size_t>& tup) const
    {
        size_t a = std::get<0>(tup);
        size_t b = std::get<1>(tup);
        size_t c = std::get<2>(tup);
        std::hash<size_t> obj;
        return ((obj(a) ^ (obj(b) << 1)) >> 1) ^ obj(c);
    }
};


namespace utility
{
    struct color_vert
    {
        glm::vec3 pos;
        glm::vec3 color;
    };

    struct vertex_vtn
    {
        glm::vec3 v;
        glm::vec2 t;
        glm::vec3 n;
    };

    struct vertex_vn
    {
        glm::vec3 v;
        glm::vec3 n;
    };

    void print(glm::mat4 mat);
    void print(glm::mat3 mat);
    void print(glm::vec4 mat);
    void print(glm::vec3 mat);
    void print(glm::vec2 mat);
    std::string unix_file(std::istringstream& iss);
    glm::vec3 make_vec3(std::istringstream& iss, const char* func_name);
    glm::vec2 make_vec2(std::istringstream& iss, const char* func_name);
    float make_float(std::istringstream& iss, const char* func_name);

}

#endif //UTILITY_HPP
