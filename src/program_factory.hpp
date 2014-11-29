#ifndef PROGRAM_FACTORY_HPP
# define PROGRAM_FACTORY_HPP

# include "program.hpp"

enum program_type
{
    DEFERRED_SPOT_LIGHT,
    DEFERRED_DIR_LIGHT,
    NUMBER_OF_PROGRAMS
};

class program_factory
{
public:
    // This function should also call the init function of the programs
    program* generate(enum program_type type);
    GLuint compile_shader(const char *shader_file, GLenum shader_type);
    void clear_cache();
private:
    // Indices to know which shader object to reuse for which program
    unsigned vertex_idx[NUMBER_OF_PROGRAMS] = { 0, 0 };
    unsigned fragment_idx[NUMBER_OF_PROGRAMS] = { 0, 1 };
    GLuint vertex_shaders[NUMBER_OF_PROGRAMS] = { 0, 0 };
    GLuint fragment_shaders[NUMBER_OF_PROGRAMS] = { 0, 0 };
    GLuint program_ids[NUMBER_OF_PROGRAMS] = { 0, 0 };
    // Removes all references to previously compiled shaders by releasing those
    // shaders.  However all current programs stay valid as long as
    // glLinkProgram is not called.
    void load_shader(GLuint program_id, const char* shader_file,
                     GLenum shader_type, GLuint& shader);
};

#endif // PROGRAM_FACTORY_HPP
