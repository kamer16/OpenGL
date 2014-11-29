#include "program_factory.hpp"
#include "utility.hpp"
#include "shader.hpp"
#include "textfile.hpp"

#define SRC_DIR_LIGHT_FRAG ("src/shaders/dir_light.frag")
#define SRC_SPOT_LIGHT_FRAG ("src/shaders/spot_light.frag")

// TODO make uniform name for deferred_light_vs
#define SRC_SPOT_LIGHT_VERT ("src/shaders/spot_light.vert")

static const char *g_vertex_source[] =
{
    SRC_SPOT_LIGHT_VERT
};
static const char *g_fragment_source[] =
{
    SRC_SPOT_LIGHT_FRAG,
    SRC_DIR_LIGHT_FRAG
};

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

    // TODO add indexed vector to guess render_type
    program* res = new program(program_id, render_type::stencil);
    res->init();
    return res;
}

void
program_factory::clear_cache()
{
    // TODO remember to which program id's shader have been atteched to release
    // them
    for (unsigned i = 0; i < NUMBER_OF_PROGRAMS; ++i) {
        // If no program was created there is now point in detaching shader
        if (!program_ids[i])
            continue;
        glDetachShader(program_ids[i], vertex_shaders[vertex_idx[i]]);
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
