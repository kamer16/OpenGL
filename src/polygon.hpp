#ifndef POLYGONS_HPP
# define POLYGONS_HPP

# include <GL/glew.h>
# include <array>

class polygon;

polygon *make_coordinate_polygon(GLuint program_id);
polygon *make_quad_xz_polygon(GLuint program_id);
polygon *make_cube_polygon(GLuint program_id);

class polygon
{
public:
    polygon(GLenum mode);
    void draw();

    template <typename T, std::size_t N>
    void load_index_buffer(std::array<T, N> &cubeIdxs);

    template <typename T, std::size_t N> void
    load_vertex_buffer(GLuint program_id, std::array<T, N> &cubeVerts);

    friend polygon *make_coordinate_polygon(GLuint program_id);
    friend polygon *make_quad_xz_polygon(GLuint program_id);
    friend polygon *make_cube_polygon(GLuint program_id);

private:
    GLint nb_elt_;
    GLuint vao_id_;
    GLuint index_buffer_id_;
    GLuint vert_buffer_id_;
    GLenum mode_;
};

# include "polygon.hxx"

#endif // POLYGONS_HPP
