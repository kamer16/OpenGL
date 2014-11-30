#include "program_factory.hpp"
#include "utility.hpp"
#include "shader.hpp"
#include "textfile.hpp"

#define SRC_DIR_LIGHT_FRAG ("src/shaders/dir_light.frag")
#define SRC_SPOT_LIGHT_FRAG ("src/shaders/spot_light.frag")
#define SRC_POINT_LIGHT_FRAG ("src/shaders/pos_light.frag")

#define SRC_STENCIL_VERT ("src/shaders/stencil.vert")
#define SRC_STENCIL_FRAG (NULL)

// Geometry sources
#define SRC_COL_VERT ("src/shaders/color.vert")
#define SRC_COL_FRAG ("src/shaders/color.frag")
#define SRC_MAT_VERT ("src/shaders/material.vert")
#define SRC_MAT_FRAG ("src/shaders/material.frag")
#define SRC_BUMP_VERT ("src/shaders/bump.vert")
#define SRC_BUMP_FRAG ("src/shaders/bump.frag")
#define SRC_DISS_VERT ("src/shaders/dissolve.vert")
#define SRC_DISS_FRAG ("src/shaders/dissolve.frag")
#define SRC_BUMP_DISS_FRAG ("src/shaders/bump_dissolve.frag")
#define SRC_GEO_FRAG ("src/shaders/geometry.frag")


// TODO make uniform name for deferred_light_vs
#define SRC_SPOT_LIGHT_VERT ("src/shaders/spot_light.vert")

// DISOLVE_VERT == GEO_VERT
// BUMP_VERT == BUMP_DISO_VERT
static const char *g_vertex_source[] =
{
    // deferred light
    SRC_SPOT_LIGHT_VERT,
    // stencil pass
    SRC_STENCIL_VERT,
    // gemetry pass
    SRC_COL_VERT,
    SRC_MAT_VERT,

    // TODO change name
    SRC_DISS_VERT,

    // TODO change name
    SRC_BUMP_VERT
};
static const char *g_fragment_source[] =
{
    // deferred light shaders
    SRC_SPOT_LIGHT_FRAG,
    SRC_DIR_LIGHT_FRAG,
    SRC_POINT_LIGHT_FRAG,
    // scencil frag (NULL)
    SRC_STENCIL_FRAG,
    // geometry pass
    SRC_COL_FRAG,
    SRC_MAT_FRAG,

    SRC_DISS_FRAG,
    SRC_GEO_FRAG,

    SRC_BUMP_FRAG,
    SRC_BUMP_DISS_FRAG
};
static const render_type g_render_type[NUMBER_OF_PROGRAMS] =
{
    // Light pass that use the stencil pass
    render_type::stencil,
    render_type::stencil,
    render_type::stencil,
    // Stencil pass
    render_type::stencil,
    // Geometry pass
    render_type::color,
    render_type::material,

    render_type::dissolve,
    render_type::basic,

    render_type::bump,
    render_type::bump_dissolve
};

program_factory::program_factory()
{
    for (unsigned i = 0; i < NUMBER_OF_PROGRAMS; ++i) {
        vertex_shaders[i] = 0;
        fragment_shaders[i] = 0;
        program_ids[i] = 0;
    }
}

GLuint
program_factory::compile_shader(const char *shader_file, GLenum shader_type)
{
    char *shader_string = NULL;

    GLuint shader = glCreateShader(shader_type);

    shader_string = textFileRead(shader_file);

    glShaderSource(shader, 1, &shader_string, NULL);
    delete[] shader_string;

    glCompileShader(shader);

    printShaderInfoLog(shader, shader_file);

    return shader;
}

void
program_factory::load_shader(GLuint program_id, const char* shader_file,
                             GLenum shader_type, GLuint& shader)
{
    if (!shader_file)
        return;
    // shader value zero is used to check is shader was already compiled
    if (!shader)
        shader = compile_shader(shader_file, shader_type);
    glAttachShader(program_id, shader);
}

program*
program_factory::generate(enum program_type type)
{
    GLuint program_id = glCreateProgram();

    unsigned v_idx = vertex_idx[type];
    GLuint& vs = vertex_shaders[v_idx];
    load_shader(program_id, g_vertex_source[v_idx], GL_VERTEX_SHADER, vs);

    unsigned f_idx = fragment_idx[type];
    GLuint& fs = fragment_shaders[f_idx];
    load_shader(program_id, g_fragment_source[f_idx], GL_FRAGMENT_SHADER, fs);

    glLinkProgram(program_id);
    program_ids[type] = program_id;

    program* res = new program(program_id, g_render_type[type]);
    res->init();
    return res;
}

void
program_factory::clear_cache()
{
    for (unsigned i = 0; i < NUMBER_OF_PROGRAMS; ++i) {
        // If no program was created there is now point in detaching shader
        if (!program_ids[i])
            continue;
        if (vertex_shaders[vertex_idx[i]])
            glDetachShader(program_ids[i], vertex_shaders[vertex_idx[i]]);
        if (fragment_shaders[fragment_idx[i]])
            glDetachShader(program_ids[i], fragment_shaders[fragment_idx[i]]);
    }

    for (unsigned i = 0; i < NUMBER_OF_PROGRAMS; ++i) {
        GLuint& v_shader = vertex_shaders[vertex_idx[i]];
        if (v_shader) {
            glDeleteShader(v_shader);
            v_shader = 0;
        }
        GLuint& f_shader = fragment_shaders[fragment_idx[i]];
        if (f_shader) {
            glDeleteShader(f_shader);
            f_shader = 0;
        }
    }
}
