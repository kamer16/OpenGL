#include "utility.hpp"

#include <iostream>

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
    std::cout << std::endl;
}

void print(glm::mat3 mat)
{
    std::cout << mat[0].x << "\t" << mat[1].x << "\t"
              << mat[2].x << "\t" << std::endl;
    std::cout << mat[0].y << "\t" << mat[1].y << "\t"
              << mat[2].y << "\t" << std::endl;
    std::cout << mat[0].z << "\t" << mat[1].z << "\t"
              << mat[2].z << "\t" << std::endl;
    std::cout << std::endl;
}

void print(glm::vec4 vec)
{
    std::cout << vec.x << "\t" << vec.y << "\t" << vec.z << "\t" << vec.w
              << std::endl;
    std::cout << std::endl;
}

void print(glm::vec3 vec)
{
    std::cout << vec.x << "\t" << vec.y << "\t" << vec.z << std::endl;
    std::cout << std::endl;
}

void print(glm::vec2 vec)
{
    std::cout << vec.x << "\t" << vec.y  << std::endl;
    std::cout << std::endl;
}
