#ifndef PROGRAM_FACTORY_HPP
# define PROGRAM_FACTORY_HPP

# include "program.hpp"

enum program_type
{
    // Light pass
    DEFERRED_SPOT_LIGHT,
    DEFERRED_DIR_LIGHT,
    DEFERRED_POINT_LIGHT,
    // Stencil pass
    STENCIL_PASS,
    // Geometry pass
    GEOM_COLOR_PASS,
    GEOM_MATERIAL_PASS,

    GEOM_DISSOLVE_PASS,
    GEOM_BASIC_PASS,

    GEOM_BUMP_PASS,
    GEOM_BUMP_DISSOLVE_PASS,

    SHADOW_MAP_PASS,

    NUMBER_OF_PROGRAMS
};

class program_factory
{
public:
    // This function should also call the init function of the programs
    program* generate(enum program_type type);
    program_factory();
    void clear_cache();
    void load_shader(GLuint program_id, const char* shader_file,
                     GLenum shader_type, GLuint& shader);
private:
    // Indices to know which shader object to reuse for which program
    unsigned vertex_idx[NUMBER_OF_PROGRAMS] =
    {
        // Deferred light pass
        0, 0, 0,
        // Stencil pass
        1,
        // Geometry pass,
        2, 3, 4, 4, 5, 5,
        // Shadow map
        6
    };
    unsigned fragment_idx[NUMBER_OF_PROGRAMS] =
    {
        // Deferred light pass
        0,
        // Stencil pass
        1, 2, 3,
        // Geometry pass,
        4, 5, 6, 7, 8, 9,
        // Shadow map (NULL) like stencil fragment
        3
    };
    GLuint vertex_shaders[NUMBER_OF_PROGRAMS];
    GLuint fragment_shaders[NUMBER_OF_PROGRAMS];
    GLuint program_ids[NUMBER_OF_PROGRAMS];
    // Removes all references to previously compiled shaders by releasing those
    // shaders.  However all current programs stay valid as long as
    // glLinkProgram is not called.
    GLuint compile_shader(const char *shader_file, GLenum shader_type);
    void printProgramInfoLog(GLuint obj);
    void printShaderInfoLog(GLuint obj, const char *file);
};

#endif // PROGRAM_FACTORY_HPP
