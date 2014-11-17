#ifndef RENDER_SCENE_CPP
# define RENDER_SCENE_CPP

# include <GL/glew.h> // GLuint
# include <vector>

# include <glm/glm.hpp>
# include <glm/gtc/type_ptr.hpp> /* value_ptr */

# include <GLFW/glfw3.h>

# include <memory>

# include "devices_state.hpp"
# include "resource_manager.hpp"
# include "object.hpp"
# include "camera.hpp"
# include "program.hpp"
# include "light.hpp"
# include "polygon.hpp"

class scene
{
public:
    using lights = std::vector<light*>;
    using objects = std::vector<object*>;
    scene(float aspect_ratio);
    // Update model matrix and light position accordingly.
    // Set the shaders with the updated transformation matrix and light
    // position.
    // Draw each object
    void draw(program& program);
    // Function used for drawing elments in deferred rendering mode
    void draw_geometry(program& program);
    // Function used for drawing light sources in deferred rendering mode
    void draw_lights(program& program);
    // Check for devices inputs, and update camera position
    void update(const devices_state &device);
    void add_object(object *object);
    void add_light(light* light);
    void init(std::shared_ptr<resource_manager> rm);
    ~scene();

private:
    // Transforms light position into eye coordinate space and send it to shader
    // Activate ambiant, specular, diffuse parameter in shader
    void set_light_source();

    // \param model_mat, the object's being drawn model matrix
    void set_model_view_matrix(const glm::mat4& model_mat);
    // Activate ambiant, specular, diffuse parameter for light in shader
    void set_light_color();

    camera camera_;

    objects objects_;
    polygon *quad_xy;
    lights lights_;
};

#endif // RENDER_SCENE_CPP
