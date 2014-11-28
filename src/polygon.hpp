#ifndef POLYGONS_HPP
# define POLYGONS_HPP

# include <GL/glew.h>
# include <array>

# include "object.hpp"
# include "program.hpp"
# include "utility.hpp"

class polygon;

polygon* make_coordinate_polygon();
polygon* make_quad_xz_polygon();
polygon* make_quad_xy_polygon();
polygon* make_cube_polygon();
polygon* make_sphere_polygon(unsigned stacks, unsigned slices, float radius = 1);

// TODO remove inheritance, leave object class for complexe objects
class polygon : public object
{
public:
    static const bool has_normal = 0;
    static const bool has_texture = 0;
    static const bool has_adjacent = 0;
    using value_type = utility::vertex_vn;
    using container_vn = std::vector<utility::vertex_vn>;
    using vertices_idx = std::vector<unsigned>;
    polygon(GLenum mode);
    virtual ~polygon() override;
    virtual void draw(program& program) override;
    container_vn& get_vertices();
    vertices_idx& get_indices();
    auto get_resource() -> element_resource&;

private:
    friend polygon* make_coordinate_polygon();
    friend polygon* make_quad_xz_polygon();
    friend polygon* make_quad_xy_polygon();
    friend polygon* make_cube_polygon();

    element_resource resource_;
    container_vn vertices_;
    vertices_idx indices_;
};

#endif // POLYGONS_HPP
