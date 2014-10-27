#ifndef POLYGONS_HPP
# define POLYGONS_HPP

# include <GL/glew.h>
# include <array>

# include "object.hpp"

object *make_coordinate_polygon(GLuint program_id);
object *make_quad_xz_polygon(GLuint program_id);
object *make_cube_polygon(GLuint program_id);

// TODO remove inheritance, leave object class for complexe objects
class polygon : public object
{
public:
    polygon(GLenum mode);
    virtual ~polygon() override;
    virtual void draw() override;

private:
    friend object *make_coordinate_polygon(GLuint program_id);
    friend object *make_quad_xz_polygon(GLuint program_id);
    friend object *make_cube_polygon(GLuint program_id);
    template <typename T, std::size_t N>
    void load_index_buffer(std::array<T, N> &cubeIdxs);
    template <typename T, std::size_t N> void
    load_vertex_buffer(GLuint program_id, std::array<T, N> &cubeVerts);

    GLint nb_elt_;
    GLuint index_buffer_id_;
    GLenum mode_;
};

# include "polygon.hxx"

#endif // POLYGONS_HPP
