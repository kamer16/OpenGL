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

    struct vertex_vnt
    {
        glm::vec3 v;
        glm::vec3 n;
        glm::vec2 t;
        static const bool has_texture = 1;
        static const bool has_adjacent = 0;
        vertex_vnt(glm::vec3& v_, glm::vec3& n_, glm::vec2& t_)
            : v(v_), n(n_), t(t_)
        { }
    };

    struct vertex_vnta
    {
        glm::vec3 v;
        glm::vec3 n;
        glm::vec2 t;
        glm::vec4 a;
        static const bool has_texture = 1;
        static const bool has_adjacent = 1;
        vertex_vnta(glm::vec3& v_, glm::vec3& n_, glm::vec2& t_)
            : v(v_), n(n_), t(t_)
        { }
    };

    struct vertex_vn
    {
        glm::vec3 v;
        glm::vec3 n;
        static const bool has_texture = 0;
        static const bool has_adjacent = 0;
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
