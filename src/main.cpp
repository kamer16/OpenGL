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

#include <iostream>

static void enableEnv()
{
}

static void generate_lights(std::shared_ptr<resource_manager> rm, scene& scene)
{
    for (unsigned i = 0; i < 4; ++i) {
        pos_light* p = new pos_light();
        object* cube = polygon::make_cube();
        rm->load_indexed_object(*cube);
        p->set_diffuse(glm::vec4(1));
        p->set_specular(glm::vec4(1));
        p->set_quadratic_att(0.00001f);
        p->set_linear_att(0.00001f);
        float z = i < 2 ? -1 : 1;
        float x = i == 1  || i == 3 ? -1 : 1;
        float depth = 500;
        p->set_position(glm::vec4(depth * x, i * depth / 2, depth * z, 1));
        cube->translate(glm::vec3(depth * x, i * depth / 2, depth * z));
        cube->scale(glm::vec3(5));
        scene.add_light(p);
        scene.add_object(cube);
    }
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

    program_factory prog_fact;

    program* p1 = prog_fact.generate(GEOM_MATERIAL_PASS);
    program* p2 = prog_fact.generate(GEOM_COLOR_PASS);
    program* p3 = prog_fact.generate(GEOM_BUMP_PASS);
    program* p4 = prog_fact.generate(GEOM_DISSOLVE_PASS);
    program* p5 = prog_fact.generate(GEOM_BUMP_DISSOLVE_PASS);
    // TODO change name
    program* p7 = prog_fact.generate(GEOM_GEOMETRY_PASS);
    // The light pass renders stencil objects to apply lighting equations only
    // on the affectet objects that have been loaded in multiple textures
    program* p8 = prog_fact.generate(DEFERRED_DIR_LIGHT);
    program* p9 = prog_fact.generate(DEFERRED_POINT_LIGHT);
    program* p10 = prog_fact.generate(DEFERRED_SPOT_LIGHT);
    // The shader updates the stencil buffer
    program* p11 = prog_fact.generate(STENCIL_PASS);
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

    /* Loop until the user closes the window */
    scene scene1(aspect_ratio);
    scene1.init(rm);
    scene1.add_light(light_spot_default_new());
    scene1.add_light(light_pos_default_new());
    scene1.add_light(light_dir_default_new());
    generate_lights(rm, scene1);
    scene1.add_object(obj);
    object* coord = polygon::make_coordinate();
    rm->load_indexed_object(*coord);
    scene1.add_object(coord);
    object* cube = polygon::make_cube();
    rm->load_indexed_object(*cube);
    cube->scale(glm::vec3(10, 10, 10));
    cube->translate(glm::vec3(0, 0, 20));
    scene1.add_object(cube);


    fps_manager& fps_manager = fps_manager::get_instance();
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
        // After geometry pass, no one shouldl write in depth buffer.  However,
        // this does not prevent the stencil pass from reading it.
        glDepthMask(GL_FALSE);

        glEnable(GL_STENCIL_TEST);
        scene1.draw_pos_lights(*p9, *p11, fb);
        scene1.draw_spot_lights(*p10, *p11, fb);

        // Directionnal light does not need a stencil buffer, also the geometry
        // pass also needs the stencil to be disactivated
        glDisable(GL_STENCIL_TEST);
        scene1.draw_dir_lights(*p8, fb);


        fb.final_pass(opt.window_width, opt.window_height);
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
