#include <GL/glew.h>

#include "render_scene.hpp"

scene::scene(float aspect_ratio)
    : camera_(aspect_ratio)
{
    quad_xy = make_quad_xy_polygon();
}

void
scene::init(std::shared_ptr<resource_manager> rm)
{
    rm->load_indexed_polygon(*quad_xy);
}

void
scene::draw_pos_lights(program& program)
{
    program.use();
    for (auto light : pos_lights_) {
        program.bind_light(*light, camera_.get_view_mat());
        program.bind_mvp(glm::mat4());
        quad_xy->draw(program);
    }
}

void
scene::draw_dir_lights(program& program)
{
    program.use();
    for (auto light : dir_lights_) {
        program.bind_light(*light, camera_.get_view_mat());
        program.bind_mvp(glm::mat4());
        quad_xy->draw(program);
    }
}

void
scene::draw_spot_lights(program& program)
{
    program.use();
    for (auto light : spot_lights_) {
        program.bind_light(*light, camera_.get_view_mat());
        program.bind_mvp(glm::mat4());
        quad_xy->draw(program);
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
