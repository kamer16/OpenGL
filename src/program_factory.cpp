#include "program_factory.hpp"
#include "utility.hpp"
#include "shader.hpp"

#define SRC_DIR_LIGHT_VERT ("src/shaders/dir_light.vert")
#define SRC_DIR_LIGHT_FRAG ("src/shaders/dir_light.frag")
#define SRC_SPOT_LIGHT_VERT ("src/shaders/spot_light.vert")
#define SRC_SPOT_LIGHT_FRAG ("src/shaders/spot_light.frag")

program*
program_factory::generate_deferred_spot_light()
{
    GLuint program_id;
    load_shaders(SRC_SPOT_LIGHT_VERT, SRC_SPOT_LIGHT_FRAG, &program_id);
    return new program(program_id, render_type::stencil);
}


program*
program_factory::generate_deferred_dir_light()
{
    GLuint program_id;
    load_shaders(SRC_DIR_LIGHT_VERT, SRC_DIR_LIGHT_FRAG, &program_id);
    return new program(program_id, render_type::stencil);
}
