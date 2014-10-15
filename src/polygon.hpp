#ifndef POLYGONS_HPP
# define POLYGONS_HPP

# include <GL/glew.h>
# include <array>

# include "render_scene.hpp"

class polygon
{
public:
    static polygon *coordinate_new(GLuint program_id);
    static polygon *quad_xz_new(GLuint program_id);

    polygon(GLenum mode);
    void draw();

    template <typename T, std::size_t N>
    void load_index_buffer(std::array<T, N> &cubeIdxs);

    template <typename T, std::size_t N> void
    load_vertex_buffer(GLuint program_id, std::array<T, N> &cubeVerts);

private:
    GLint nb_elt_;
    GLuint vao_id_;
    GLuint index_buffer_id_;
    GLuint vert_buffer_id_;
    GLenum mode_;
};

# include "polygon.hxx"

#endif // POLYGONS_HPP
