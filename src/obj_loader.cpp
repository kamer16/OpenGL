#include <fstream> // ifstream
#include <iostream>

#include "obj_loader.hpp"

void load_obj(const char *file)
{
    std::ifstream instr;
    std::string token;
    instr.open(file);
    char buff[256];
    if (instr.bad())
      return;
    instr >> token;
    while (!instr.eof()) {
      if (!token.compare("#")) {
        instr.getline(buff, 256);
        std::cout << "Comment: " << buff << std::endl;
      }
      else if (!token.compare("v")) {
          float x, y, z;
          instr >> x;
          instr >> y;
          instr >> z;
          std::cout << "Vertex " << x << ',' << y << ',' << z <<  std::endl;
      }
      else if (!token.compare("vt")) {
          float s, t;
          instr >> s;
          instr >> t;
          std::cout << "Texture " << s << ',' << t << std::endl;
      }
      else if (!token.compare("vn")) {
          float x, y, z;
          instr >> x;
          instr >> y;
          instr >> z;
          std::cout << "Normal " << x << ',' << y << ',' << z <<  std::endl;
      }
      else if (!token.compare("f")) {
          std::string v1, v2, v3, v4;
          instr >> v1;
          instr >> v2;
          instr >> v3;
          instr >> v4;
          unsigned idx_v, idx_st, idx_n;
          sscanf(v1.c_str(), "%u/%u/%u", &idx_v, &idx_st, &idx_n);
          std::cout << idx_v << idx_st << idx_n << ' ';
          sscanf(v2.c_str(), "%u/%u/%u", &idx_v, &idx_st, &idx_n);
          std::cout << idx_v << idx_st << idx_n << ' ';
          sscanf(v3.c_str(), "%u/%u/%u", &idx_v, &idx_st, &idx_n);
          std::cout << idx_v << idx_st << idx_n << ' ';
          sscanf(v4.c_str(), "%u/%u/%u", &idx_v, &idx_st, &idx_n);
          std::cout << idx_v << idx_st << idx_n << ' ';
          std::cout << std::endl;
      }
      instr >> token;
    }
}
