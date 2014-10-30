#ifndef SHADER_HPP
# define SHADER_HPP

#include <GL/glew.h> // GLuint

// TODO wrap in a class, maybe in a program class
/* Compile shaders and return the id of the newly created program */
void load_shaders(const char *vert_file, const char *frag_file,
                 GLuint *program_id);

#endif // SHADER_HPP
