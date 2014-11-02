#include <GL/glew.h>

#include "render_scene.hpp"

scene::scene(float aspect_ratio)
    : camera_(aspect_ratio)
{
}

void
scene::update_and_draw(const devices_state &device, program& program)
{
    program.use();
    camera_.update(device);
    program.bind_lights(camera_.get_view_mat(), lights_);
    for (auto obj : objects_) {
        const glm::mat4& model_mat = obj->get_model_mat();
        program.bind_scene(model_mat, camera_.get_view_mat(),
                           camera_.get_proj_mat());
        obj->draw(program);
    }
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
