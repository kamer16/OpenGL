#include <GL/glew.h>
#include <array>

#include "vbo.hpp"

struct Vec2 { float x, y; };
struct Vec3 { float x, y, z; };
struct Vert { Vec3 pos; Vec2 tex; };

// Returns the buffers handler id
GLuint load_vertex_buffer(GLuint program_id)
{
  std::array<Vert, 8> cubeVerts = {{
    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } }, { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } },
    { {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } }, { {  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
    { { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f } }, { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f } },
    { { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f } }, { { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f } }
  }};

  GLuint vertBufferID;
  glGenBuffers(1, &vertBufferID);

  GLuint pos_location = glGetAttribLocation(program_id, "in_position");
  GLuint uv_idx = glGetAttribLocation(program_id, "in_uv");
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
  unsigned byte_size = sizeof (Vert) * cubeVerts.size();
  glBufferData(GL_ARRAY_BUFFER, byte_size, cubeVerts.data(), GL_STATIC_DRAW);

  // Sets shaders attribute for vertices positions
  glEnableVertexAttribArray(pos_location); // Matches layout (location = 0)
  glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), 0);

  // Sets shaders attribute for texture coordinates
  glEnableVertexAttribArray(uv_idx); // Matches layout (location = 1)
  glVertexAttribPointer(uv_idx, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (GLvoid*)sizeof(Vec3));

  return vertBufferID;
}

// Returns the index buffer handler id
GLuint load_index_buffer()
{
  std::array<unsigned int, 36> cubeIdxs = {{
    0, 2, 1, 0, 3, 2, // Right
    4, 5, 6, 4, 6, 7, // Left
    0, 7, 3, 0, 4, 7, // Top
    1, 6, 2, 1, 5, 6, // Bottom
    0, 5, 1, 0, 4, 5, // Front
    3, 7, 6, 3, 6, 2  // Back
  }};

  GLuint indexBufferID;
  glGenBuffers(1, &indexBufferID);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
  unsigned byte_size = sizeof (unsigned int) * cubeIdxs.size();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, byte_size, cubeIdxs.data(), GL_STATIC_DRAW);

  return indexBufferID;
}

void BindArrays(GLuint program_id, GLuint *vaoID)
{
  glGenVertexArrays(2, vaoID);

  glBindVertexArray(vaoID[0]);
  GLuint vertBufferID = load_vertex_buffer(program_id);
  GLuint indexBufferID = load_index_buffer();

  glBindVertexArray(vaoID[1]);
  GLuint vertBufferID2 = load_vertex_buffer(program_id);
  GLuint indexBufferID2 = load_index_buffer();

  // TODO we need to delete the buffers when no longer needed
  (void) vertBufferID;
  (void) vertBufferID2;
  (void) indexBufferID;
  (void) indexBufferID2;

  glBindVertexArray(0);
}
