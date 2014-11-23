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
#include "resource_manager.hpp" // set_shader_uniforms

#include "obj_loader.hpp"
#include "g_buffer.hpp"

#include <iostream>

#define SRC_COL_VERT ("src/shaders/color.vert")
#define SRC_COL_FRAG ("src/shaders/color.frag")

#define SRC_MAT_VERT ("src/shaders/material.vert")
#define SRC_MAT_FRAG ("src/shaders/material.frag")
#define SRC_BUMP_VERT ("src/shaders/bump.vert")
#define SRC_BUMP_FRAG ("src/shaders/bump.frag")
#define SRC_DISS_VERT ("src/shaders/dissolve.vert")
#define SRC_DISS_FRAG ("src/shaders/dissolve.frag")
#define SRC_BUMP_DISS_VERT ("src/shaders/bump_dissolve.vert")
#define SRC_BUMP_DISS_FRAG ("src/shaders/bump_dissolve.frag")

#define SRC_GEO_VERT ("src/shaders/geometry.vert")
#define SRC_GEO_FRAG ("src/shaders/geometry.frag")
#define SRC_DIR_LIGHT_VERT ("src/shaders/dir_light.vert")
#define SRC_DIR_LIGHT_FRAG ("src/shaders/dir_light.frag")
#define SRC_POS_LIGHT_VERT ("src/shaders/pos_light.vert")
#define SRC_POS_LIGHT_FRAG ("src/shaders/pos_light.frag")
#define SRC_CONE_LIGHT_VERT ("src/shaders/spot_light.vert")
#define SRC_CONE_LIGHT_FRAG ("src/shaders/spot_light.frag")

static void enableEnv()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
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

    program p1(SRC_MAT_VERT, SRC_MAT_FRAG, render_type::material);
    program p2(SRC_COL_VERT, SRC_COL_FRAG, render_type::color);
    program p3(SRC_BUMP_VERT,SRC_BUMP_FRAG, render_type::bump);
    program p4(SRC_DISS_VERT, SRC_DISS_FRAG, render_type::dissolve);
    program p5(SRC_BUMP_DISS_VERT, SRC_BUMP_DISS_FRAG, render_type::bump_dissolve);
    program p7(SRC_GEO_VERT, SRC_GEO_FRAG, render_type::basic);
    program p8(SRC_DIR_LIGHT_VERT, SRC_DIR_LIGHT_FRAG, render_type::basic);
    program p9(SRC_POS_LIGHT_VERT, SRC_POS_LIGHT_FRAG, render_type::basic);
    program p10(SRC_CONE_LIGHT_VERT, SRC_CONE_LIGHT_FRAG, render_type::basic);
    p1.init(); p2.init(); p3.init(); p4.init(); p5.init(); p7.init(); p8.init();
    p9.init(); p10.init();
    p8.bind_screen_dimension(opt.window_width, opt.window_height);
    p9.bind_screen_dimension(opt.window_width, opt.window_height);
    p10.bind_screen_dimension(opt.window_width, opt.window_height);
    obj_loader loader;
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
    scene1.add_object(obj);
    scene scene2(aspect_ratio);
    scene2.init(rm);
    polygon* coord = make_coordinate_polygon();
    rm->load_indexed_polygon(*coord);
    polygon* quad = make_quad_xz_polygon();
    polygon* sphere = make_sphere_polygon(20, 20, 40);
    polygon* sphere2 = make_sphere_polygon(10, 10, 1);
    sphere2->translate(glm::vec3(80, 0, 0));
    sphere2->scale(glm::vec3(40, 40, 40));
    rm->load_indexed_polygon(*quad);
    rm->load_indexed_polygon(*sphere);
    rm->load_indexed_polygon(*sphere2);
    scene2.add_object(coord);
    //scene2.add_object(quad);
    scene2.add_object(sphere);
    scene2.add_object(sphere2);
    polygon* cube = make_cube_polygon();
    rm->load_indexed_polygon(*cube);
    cube->scale(glm::vec3(10, 10, 10));
    cube->translate(glm::vec3(0, 0, 20));
    scene2.add_object(cube);


    fps_manager& fps_manager = fps_manager::get_instance();
    while (!glfwWindowShouldClose(window))
    {
        const devices_state &device = devices_state::get_instance(window);

        // Write data to current framebuffer
        fb.bind_for_writing();
        glDepthMask(GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        scene1.update(device);
        // Draw basic textures
        scene1.draw_geometry(p7);
        // Draw bump maps
        scene1.draw_geometry(p3);
        // Draw bump with dissolve maps
        scene1.draw_geometry(p5);
        // Draw material objects
        scene1.draw_geometry(p1);
        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);


        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
        fb.bind_for_reading();
        glClear(GL_COLOR_BUFFER_BIT);
        scene1.draw_dir_lights(p8);
        scene1.draw_pos_lights(p9);
        scene1.draw_spot_lights(p10);

        // update and draw scene2
        scene2.update(device);
        // Draw basic
        scene2.draw(p2);

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
