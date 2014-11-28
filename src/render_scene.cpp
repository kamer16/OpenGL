#include <GL/glew.h>

#include "render_scene.hpp"

scene::scene(float aspect_ratio)
    : camera_(aspect_ratio)
{
    quad_xy = polygon::make_quad_xy();
    sphere = polygon::make_sphere(20, 20, 1);
}

void
scene::init(std::shared_ptr<resource_manager> rm)
{
    rm->load_indexed_object(*quad_xy);
    quad_xy->set_render_mode(render_type::stencil);
    rm->load_indexed_object(*sphere);
    sphere->set_render_mode(render_type::stencil);
}

template <typename light_t>
void scene::positional_stencil_pass(program& program, light_t* light,
                                    g_buffer& fbo)
{
    fbo.bind_for_stencil_pass();

    glEnable(GL_DEPTH_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glDisable(GL_CULL_FACE);

    // We need the stencil test to be enabled but we want it
    // to succeed always. Only the depth test matters.
    glStencilFunc(GL_ALWAYS, 0, 0);

    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

    program.bind_light(*light, camera_.get_view_mat());
    glm::mat4 vp_mat = camera_.get_proj_mat() * camera_.get_view_mat();
    glm::mat4 model_mat = glm::translate(glm::vec3(light->get_position()));
    float scale = light->get_scale();
    model_mat = glm::scale(model_mat, glm::vec3(scale, scale, scale));
    program.bind_mvp(vp_mat * model_mat);
    sphere->draw(program);
}

template <typename light_t>
void scene::positional_light_pass(program& program, light_t* light,
                                  g_buffer& fbo)
{
    light_pass(fbo);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

    program.bind_light(*light, camera_.get_view_mat());
    glm::mat4 vp_mat = camera_.get_proj_mat() * camera_.get_view_mat();
    glm::mat4 model_mat = glm::translate(glm::vec3(light->get_position()));
    float scale = light->get_scale();
    model_mat = glm::scale(model_mat, glm::vec3(scale, scale, scale));
    program.bind_mvp(vp_mat * model_mat);
    sphere->draw(program);

    glCullFace(GL_BACK);
}

void
scene::draw_pos_lights(program& light_program, program& stencil_program,
                       g_buffer& fbo)
{
    for (auto light : pos_lights_) {
        stencil_program.use();
        positional_stencil_pass(stencil_program, light, fbo);
        light_program.use();
        positional_light_pass(light_program, light, fbo);
    }
}

void
scene::light_pass(g_buffer& fbo)
{
    fbo.bind_for_light_pass();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
}

void
scene::draw_dir_lights(program& program, g_buffer& fbo)
{
    light_pass(fbo);
    program.use();
    for (auto light : dir_lights_) {
        program.bind_light(*light, camera_.get_view_mat());
        program.bind_mvp(glm::mat4());
        quad_xy->draw(program);
    }
}

void
scene::draw_spot_lights(program& light_program, program& stencil_program,
                        g_buffer& fbo)
{
    for (auto light : spot_lights_) {
        stencil_program.use();
        positional_stencil_pass(stencil_program, light, fbo);
        light_program.use();
        // TODO should use a cone to bound light
        positional_light_pass(light_program, light, fbo);
    }
}

void
scene::draw_geometry(program& program)
{
    program.use();
    for (auto obj : objects_) {
        const glm::mat4& model_mat = obj->get_model_mat();
        program.bind_scene(model_mat, camera_.get_view_mat(),
                           camera_.get_proj_mat());
        obj->draw(program);
    }
    // TODO send material shininess to shader
}

void
scene::draw(program& program)
{
    program.use();
    for (auto obj : objects_) {
        const glm::mat4& model_mat = obj->get_model_mat();
        program.bind_scene(model_mat, camera_.get_view_mat(),
                           camera_.get_proj_mat());
        obj->draw(program);
    }
}

void
scene::update(const devices_state &device)
{
    camera_.update(device);
}

void
scene::add_object(object* object)
{
    objects_.push_back(object);
}

scene::~scene()
{
    delete quad_xy;
}
