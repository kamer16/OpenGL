#include <GL/glew.h>
#include <array>

#include "vbo.hpp"
#include "utility.hpp"

// Returns the buffers handler id
template <typename T, std::size_t N>
GLuint load_vertex_buffer(GLuint program_id, std::array<T, N> &cubeVerts)
{
    GLuint vertBufferID;
    glGenBuffers(1, &vertBufferID);

    GLint pos_location = glGetAttribLocation(program_id, "in_position");
    GLint uv_idx = glGetAttribLocation(program_id, "in_uv");
    glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
    long unsigned byte_size = sizeof (T) * cubeVerts.size();
    glBufferData(GL_ARRAY_BUFFER, byte_size, cubeVerts.data(), GL_STATIC_DRAW);

    // Sets shaders attribute for vertices positions
    glEnableVertexAttribArray(pos_location); // Matches layout (location = 0)
    glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, sizeof (T), 0);

    // Sets shaders attribute for texture coordinates
    glEnableVertexAttribArray(uv_idx); // Matches layout (location = 1)
    GLvoid *offset = reinterpret_cast<GLvoid *>(sizeof (utility::vec3));
    glVertexAttribPointer(uv_idx, 2, GL_FLOAT, GL_FALSE, sizeof (T), offset);

    return vertBufferID;
}

// Returns the index buffer handler id
template <typename T, std::size_t N>
GLuint load_index_buffer(std::array<T, N> &cubeIdxs)
{
    GLuint indexBufferID;
    glGenBuffers(1, &indexBufferID);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    long unsigned byte_size = sizeof (T) * cubeIdxs.size();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, byte_size, cubeIdxs.data(), GL_STATIC_DRAW);

    return indexBufferID;
}

void BindArrays(GLuint program_id, GLuint *vaoID)
{
    std::array<unsigned int, 36> cubeIdxs = {{
        0, 2, 1, 0, 3, 2, // Right
            4, 5, 6, 4, 6, 7, // Left
            0, 7, 3, 0, 4, 7, // Top
            1, 6, 2, 1, 5, 6, // Bottom
            0, 5, 1, 0, 4, 5, // Front
            3, 7, 6, 3, 6, 2  // Back
    }};

    std::array<utility::vert, 8> cubeVerts = {{
        { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } }, { {  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f } },
            { {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } }, { {  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
            { { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f } }, { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f } },
            { { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f } }, { { -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f } }
    }};

    glGenVertexArrays(2, vaoID);

    glBindVertexArray(vaoID[0]);
    GLuint vertBufferID = load_vertex_buffer(program_id, cubeVerts);
    GLuint indexBufferID = load_index_buffer(cubeIdxs);

    std::array<unsigned int, 6> cubeIdxs2 = {{
        0, 1, 0, 2, 0, 3
    }};
    std::array<utility::vert, 8> cubeVerts2 = {{
        { {  0.0f,  0.0f,  0.0f }, { 0.5f, 0.5f } }, { {  1.0f,  0.0f, 0.0f }, { 1.0f, 0.0f } },
            { { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } }, { { 0.0f, 0.0f,  1.0f }, { 1.0f, 1.0f } }
    }};
    glBindVertexArray(vaoID[1]);
    GLuint vertBufferID2 = load_vertex_buffer(program_id, cubeVerts2);
    GLuint indexBufferID2 = load_index_buffer(cubeIdxs2);

    // TODO we need to delete the buffers when no longer needed
    (void) vertBufferID;
    (void) vertBufferID2;
    (void) indexBufferID;
    (void) indexBufferID2;

    // unbind current vertex array
    glBindVertexArray(0);
}


template <typename T>
static void
load_data(GLuint program_id, std::vector<T> &data, const char *name)
{
    GLuint buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    long unsigned byte_size = sizeof (T) * data.size();
    glBufferData(GL_ARRAY_BUFFER, byte_size, data.data(), GL_STATIC_DRAW);
    GLint location_idx = glGetAttribLocation(program_id, name);
    glEnableVertexAttribArray(location_idx); // Matches layout (location = 1)
    size_t nb_elt = sizeof (T) / sizeof (typename T::value_type);
    glVertexAttribPointer(location_idx, nb_elt, GL_FLOAT, GL_FALSE, 0, 0);
    // TODO leaking buffer
    (void) buffer_id;
}

void bind_object(GLuint program_id, GLuint *vaoID,
                 std::vector<utility::vec3> &vertices,
                 std::vector<utility::vec3> &normals,
                 std::vector<utility::vec2> &text_coords)
{
    glGenVertexArrays(1, vaoID);
    glBindVertexArray(*vaoID);

    // Sets shaders attribute for vertices positions
    load_data(program_id, vertices, "in_position");

    // Sets shaders attribute for texture coordinates
    load_data(program_id, text_coords, "in_uv");

    // Sets shaders attribute for texture coordinates
    load_data(program_id, normals, "in_norm");

    glBindVertexArray(0);
}
