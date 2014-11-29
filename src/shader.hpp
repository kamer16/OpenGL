#ifndef SHADER_HPP
# define SHADER_HPP

#include <GL/glew.h> // GLuint

// Compile shaders and return the id of the newly created program
void load_shaders(const char *vert_file, const char *frag_file,
                 GLuint *program_id);
GLuint attach_shader(const char *shader_file, GLenum shader_type,
                     GLuint program_id);
void destroy_shader(GLuint shader, GLuint program_id);

void printShaderInfoLog(GLuint obj, const char *file);

#endif // SHADER_HPP
