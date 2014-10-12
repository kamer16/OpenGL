#include <GL/glew.h>
#include <array>

#include "vbo.hpp"

void BindArrays(GLuint program, GLuint *vaoID)
{
  glGenVertexArrays(1, vaoID);

  glBindVertexArray(vaoID[0]);

  GLuint vertBufferID, indexBufferID;
  glGenBuffers(1, &vertBufferID);
  glGenBuffers(1, &indexBufferID);

  struct Vec2 { float x, y; };
  struct Vec3 { float x, y, z; };
  struct Vert { Vec3 pos; Vec2 tex; };

  std::array<Vert, 8> cubeVerts = {{
    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } }, { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } },
        { {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } }, { {  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
        { { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f } }, { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f } },
        { { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f } }, { { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f } }
  }};

  std::array<unsigned int, 36> cubeIdxs = {{
    0, 2, 1, 0, 3, 2, // Right
        4, 5, 6, 4, 6, 7, // Left
        0, 7, 3, 0, 4, 7, // Top
        1, 6, 2, 1, 5, 6, // Bottom
        0, 5, 1, 0, 4, 5, // Front
        3, 7, 6, 3, 6, 2  // Back
  }};

  // Vertex buffer
  GLuint pos_idx = glGetAttribLocation(program, "in_position");
  GLuint uv_idx = glGetAttribLocation(program, "in_uv");
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vert) * cubeVerts.size(), cubeVerts.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(pos_idx); // Matches layout (location = 0)
  glVertexAttribPointer(pos_idx, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), 0);
  glEnableVertexAttribArray(uv_idx); // Matches layout (location = 1)
  glVertexAttribPointer(uv_idx, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (GLvoid*)sizeof(Vec3));

  // Index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * cubeIdxs.size(), cubeIdxs.data(), GL_STATIC_DRAW);
  glBindVertexArray(0);
}
