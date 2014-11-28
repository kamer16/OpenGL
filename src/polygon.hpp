#ifndef POLYGONS_HPP
# define POLYGONS_HPP

# include <GL/glew.h>
# include <array>

# include "object.hpp"
# include "program.hpp"
# include "utility.hpp"

namespace polygon
{

object* make_coordinate();
object* make_quad_xz();
object* make_quad_xy();
object* make_cube();
object* make_sphere(unsigned stacks, unsigned slices, float radius = 1);

}

#endif // POLYGONS_HPP
