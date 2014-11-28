#ifndef POLYGONS_HPP
# define POLYGONS_HPP

# include <GL/glew.h>
# include <array>

# include "object.hpp"
# include "program.hpp"
# include "utility.hpp"

object* make_coordinate_polygon();
object* make_quad_xz_polygon();
object* make_quad_xy_polygon();
object* make_cube_polygon();
object* make_sphere_polygon(unsigned stacks, unsigned slices, float radius = 1);

#endif // POLYGONS_HPP
