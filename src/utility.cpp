#include "utility.hpp"

namespace utility
{

glm::vec3 make_vec3(std::istringstream& iss, const char* func_name)
{
    float x, y , z;
    iss >> x >> y >> z;
    if (iss.fail())
        std::cerr << "Unable to make vec3 for :" << func_name << std::endl;
    return glm::vec3(x, y, z);
}

glm::vec2 make_vec2(std::istringstream& iss, const char* func_name)
{
    float x, y;
    iss >> x >> y;
    if (iss.fail())
        std::cerr << "Unable to make vec2 for :" << func_name << std::endl;
    return glm::vec2(x, y);
}

float make_float(std::istringstream& iss, const char* func_name)
{
    float x;
    iss >> x;
    if (iss.fail())
        std::cerr << "Unable to make float for :" << func_name << std::endl;
    return x;
}

std::string unix_file(std::istringstream& iss)
{
    std::string file;
    iss >> file;
    for (auto c : file) {
        if (c == '\\')
            c = '/';
    }
    return file;
}

void print(glm::mat4 mat)
{
    std::cout << mat[0].x << "\t" << mat[1].x << "\t"
              << mat[2].x << "\t" << mat[3].x << std::endl;
    std::cout << mat[0].y << "\t" << mat[1].y << "\t"
              << mat[2].y << "\t" << mat[3].y << std::endl;
    std::cout << mat[0].z << "\t" << mat[1].z << "\t"
              << mat[2].z << "\t" << mat[3].z << std::endl;
    std::cout << mat[0].w << "\t" << mat[1].w << "\t"
              << mat[2].w << "\t" << mat[3].w << std::endl;
}

void print(glm::mat3 mat)
{
    std::cout << mat[0].x << "\t" << mat[1].x << "\t"
              << mat[2].x << "\t" << std::endl;
    std::cout << mat[0].y << "\t" << mat[1].y << "\t"
              << mat[2].y << "\t" << std::endl;
    std::cout << mat[0].z << "\t" << mat[1].z << "\t"
              << mat[2].z << "\t" << std::endl;
}

void print(glm::vec4 vec)
{
    std::cout << vec.x << "\t" << vec.y << "\t" << vec.z << "\t" << vec.w
              << std::endl;
}

void print(glm::vec3 vec)
{
    std::cout << vec.x << "\t" << vec.y << "\t" << vec.z << std::endl;
}

void print(glm::vec2 vec)
{
    std::cout << vec.x << "\t" << vec.y  << std::endl;
}

} // namespace utility
