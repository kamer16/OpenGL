#ifndef VBO_HPP
# define VBO_HPP

# include <vector>

# include "utility.hpp"
# include "polygon.hpp"

// Sets the id's of the array of vertex array objects
// Currently 2 id's are created
void BindArrays(GLuint program_id, GLuint *vaoID);

// From a set a vectors, bind all the data in such a way that a call to
// glDrawArrays displays the object.
void bind_object(GLuint program_id, GLuint *vaoID,
                 std::vector<utility::vec3> &vertices,
                 std::vector<utility::vec3> &normals,
                 std::vector<utility::vec2> &text_coords);

#endif // VBO_HPP
