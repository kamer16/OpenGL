#ifndef UTILITY_HPP
# define UTILITY_HPP

# include <glm/glm.hpp>
# include <sstream>
# include <iostream>
# include <string>

namespace utility
{
    struct color_vert
    {
        glm::vec3 pos;
        glm::vec3 color;
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
