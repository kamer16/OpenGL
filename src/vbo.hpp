#ifndef VBO_HPP
# define VBO_HPP

# include "polygon.hpp"

// Sets the id's of the array of vertex array objects
// Currently 2 id's are created
void BindArrays(GLuint program_id, GLuint *vaoID);

polygon * coordinate_polygon_new(GLuint program_id);

#endif // VBO_HPP
