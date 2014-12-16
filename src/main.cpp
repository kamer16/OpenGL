#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
#include <glm/gtc/type_ptr.hpp> /* value_ptr */

#include <memory> // shared_ptr

#include "render_scene.hpp" // renderScene()
#include "polygon.hpp" // polygons
#include "options.hpp" // parse_args
#include "fps_manager.hpp" // update_and_print_fps
#include "program.hpp" // init()
#include "program_factory.hpp" // generate shader programs
#include "resource_manager.hpp" // set_shader_uniforms

#include "obj_loader.hpp"
#include "g_buffer.hpp"
#include "shadow_map_fbo.hpp"

#include <iostream>

static void enableEnv()
{
}

static void generate_lights(std::shared_ptr<resource_manager> rm, scene& scene)
{
    // Point light
    for (unsigned i = 0; i < 4; ++i) {
        pos_light* p = new pos_light();
        object* sphere = polygon::make_sphere(20, 20);
        rm->load_indexed_object(*sphere);
        p->set_diffuse(glm::vec4(1));
        p->set_specular(glm::vec4(1));
        p->set_quadratic_att(0.00001f);
        p->set_linear_att(0.00001f);
        float z = i < 2 ? -1 : 1;
        float x = i == 1  || i == 3 ? -1 : 1;
        float depth = 500;
        p->set_position(glm::vec4(depth * x, 10 + i * depth / 2, depth * z, 1));
        sphere->translate(glm::vec3(depth * x, 10 + i * depth / 2, depth * z));
        sphere->scale(glm::vec3(5));
        scene.add_light(p);
        scene.add_object(sphere);
    }
    scene.add_light(light_pos_default_new());

    // Spot light
    scene.add_light(light_spot_default_new());
    object* c = polygon::make_cube();
    rm->load_indexed_object(*c);
    c->translate(glm::vec3(0.0f, 155.0f, 180.0f));
    c->scale(glm::vec3(10));
    scene.add_object(c);

    // Directional light
    scene.add_light(light_dir_default_new());
}

int main(int argc, char *argv[])
{
    options opt;
    opt.parse_args(argc, argv);
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWmonitor *monitor = opt.fullscreen ? glfwGetPrimaryMonitor() : NULL;
    float aspect_ratio = static_cast<float>(opt.window_width) /
                         static_cast<float>(opt.window_height);
    window = glfwCreateWindow(opt.window_width, opt.window_height, "OpenGL",
                              monitor, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* We are using core version, so glewExperimental needs to be set */
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    glGetError();

    if (err != GLEW_OK)
        return -1; // or handle the error in a nicer way

    enableEnv();
    g_buffer fb;
    fb.init(opt.window_width, opt.window_height);
    shadow_map_fbo sm_fb;
    sm_fb.init(opt.window_width, opt.window_height);

    // Initialize programs
    program_factory prog_fact;

    program* p1 = prog_fact.generate(GEOM_MATERIAL_PASS);
    program* p2 = prog_fact.generate(GEOM_COLOR_PASS);
    program* p3 = prog_fact.generate(GEOM_BUMP_PASS);
    program* p4 = prog_fact.generate(GEOM_DISSOLVE_PASS);
    program* p5 = prog_fact.generate(GEOM_BUMP_DISSOLVE_PASS);
    program* p7 = prog_fact.generate(GEOM_BASIC_PASS);
    // The light pass renders stencil objects to apply lighting equations only
    // on the affectet objects that have been loaded in multiple textures
    program* p8 = prog_fact.generate(DEFERRED_DIR_LIGHT);
    program* p9 = prog_fact.generate(DEFERRED_POINT_LIGHT);
    program* p10 = prog_fact.generate(DEFERRED_SPOT_LIGHT);
    // The shader updates the stencil buffer
    program* p11 = prog_fact.generate(STENCIL_PASS);
    program* p12 = prog_fact.generate(SHADOW_MAP_PASS);
    // Release shader resources from memory as they are now loaded to GPU
    prog_fact.clear_cache();
    p8->bind_screen_dimension(opt.window_width, opt.window_height);
    p9->bind_screen_dimension(opt.window_width, opt.window_height);
    p10->bind_screen_dimension(opt.window_width, opt.window_height);
    bool draw_lines = 0;
    obj_loader loader(draw_lines);
    std::shared_ptr<resource_manager> rm = std::make_shared<resource_manager>();
    object* obj = loader.load_obj(opt.mesh_file, rm);
    rm->load_indexed_object(*obj);
    if (monitor)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else // TODO somehow not working, Bug in GLFW ??
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Create scene
    scene scene1(aspect_ratio);
    scene1.init(rm);
    generate_lights(rm, scene1);
    scene1.add_object(obj);
    object* coord = polygon::make_coordinate();
    rm->load_indexed_object(*coord);
    scene1.add_object(coord);
    object* quad_xz = polygon::make_quad_xz();
    quad_xz->set_render_mode(render_type::material);
    rm->load_indexed_object(*quad_xz);
    quad_xz->translate(glm::vec3(0, -10, 0));
    quad_xz->scale(glm::vec3(1000, 1000, 1000));
    scene1.add_object(quad_xz);

    fps_manager& fps_manager = fps_manager::get_instance();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        const devices_state &device = devices_state::get_instance(window);

        // Write data to current framebuffer
        fb.start_frame();
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        scene1.update(device);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        // No stencil test required for geometry pass
        fb.bind_for_geom_pass();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Draw basic textures
        scene1.draw_geometry(*p7);
        // Draw dissolve objects
        scene1.draw_geometry(*p4);
        // Draw bump maps
        scene1.draw_geometry(*p3);
        // Draw bump with dissolve maps
        scene1.draw_geometry(*p5);
        // Draw material objects
        scene1.draw_geometry(*p1);

        sm_fb.bind_for_writing();
        //scene1.draw_shadow_spot(*p12, 0);
        scene1.draw_shadow_dir(*p12);
        sm_fb.bind_for_reading();
        // Once texture has been set, we continue writing in previous fbo
        fb.bind_for_writing();
        // After geometry pass, no one shouldl write in depth buffer.  However,
        // this does not prevent the stencil pass from reading it.
        glDepthMask(GL_FALSE);

        glEnable(GL_STENCIL_TEST);
        scene1.draw_spot_lights(*p10, *p11, fb);
        scene1.draw_pos_lights(*p9, *p11, fb);

        // Directionnal light does not need a stencil buffer, also the geometry
        // pass also needs the stencil to be disactivated
        glDisable(GL_STENCIL_TEST);
        scene1.draw_dir_lights(*p8, fb);


        fb.final_pass(opt.window_width, opt.window_height);
        sm_fb.blit_to_screen(opt.window_width, opt.window_height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // update and draw color objects which dont use lighting equations
        scene1.draw(*p2);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        GLenum error = glGetError();
        if (error)
            std::cerr << gluErrorString(error) << std::endl;
        fps_manager.update_and_set_window_title(window, "OpenGL");
    }

    glfwTerminate();
    return 0;
}
