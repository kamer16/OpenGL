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

#include <iostream>

void enableEnv();

void enableEnv()
{
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
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

    program p1("src/shaders/material.vert", "src/shaders/material.frag");
    program p2("src/shaders/color.vert", "src/shaders/color.frag");
    program p3("src/shaders/bump.vert", "src/shaders/bump.frag");
    program p4("src/shaders/bump_dissolve.vert", "src/shaders/bump_dissolve.frag");
    program p5("src/shaders/dissolve.vert", "src/shaders/dissolve.frag");
    program p6("src/shaders/default.vert", "src/shaders/default.frag");
    p1.init(); p2.init(); p3.init(); p4.init(); p5.init();
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
    scene1.add_light(light_dir_default_new());
    scene1.add_object(obj);
    scene scene2(aspect_ratio);
    polygon* coord = make_coordinate_polygon();
    rm->load_indexed_polygon(*coord);
    polygon* quad = make_quad_xz_polygon();
    rm->load_indexed_polygon(*quad);
    scene2.add_object(coord);
    scene2.add_object(quad);
    polygon* cube = make_cube_polygon();
    rm->load_indexed_polygon(*cube);
    cube->scale(glm::vec3(10, 10, 10));
    cube->translate(glm::vec3(0, 0, 20));
    scene2.add_object(cube);

    fps_manager& fps_manager = fps_manager::get_instance();
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        const devices_state &device = devices_state::get_instance(window);

        /* Swap front and back buffers */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene1.update_and_draw(device, p1);

        // update and draw scene2
        scene2.update_and_draw(device, p2);
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
