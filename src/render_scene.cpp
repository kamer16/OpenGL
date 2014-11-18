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
scene::draw_lights(program& program)
{
    program.use();
    for (auto light : lights_) {
        program.bind_dir_light(*light, camera_.get_view_mat());
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
    program.bind_lights(camera_.get_view_mat(), lights_);
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

void
scene::add_light(light* light)
{
    lights_.push_back(light);
}

scene::~scene()
{
    delete quad_xy;
}
