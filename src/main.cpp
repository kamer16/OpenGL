#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
#include <glm/gtc/type_ptr.hpp> /* value_ptr */

#include "shader.hpp" // loadShaders()
#include "texture.hpp" // loadTextures()
#include "vbo.hpp" // bind_object()
#include "render_scene.hpp" // renderScene()
#include "polygon.hpp" // polygons
#include "options.hpp" // parse_args
#include "fps_manager.hpp" // update_and_print_fps

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
    GLuint program_ids[2];
    loadShaders("src/shaders/height.vert", "src/shaders/height.frag",
                &program_ids[0]);
    loadShaders("src/shaders/color.vert", "src/shaders/color.frag",
                &program_ids[1]);

    // TODO loads texture but not used
    loadTextures(program_ids[0]);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> text_coords;
    obj_loader loader;
    using objects = std::vector<object*>;
    objects* objs = loader.load_obj(opt.mesh_file);
    for (auto obj : *objs)
        obj->bind_vao(program_ids[0]);
    if (monitor)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else // TODO somehow not working, Bug in GLFW ??
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    /* Loop until the user closes the window */
    scene scene1(program_ids[0], aspect_ratio);
    for (auto obj : *objs)
        scene1.add_object(obj);
    scene scene2(program_ids[1], aspect_ratio);
    scene2.add_object(make_coordinate_polygon(program_ids[1]));
    scene2.add_object(make_quad_xz_polygon(program_ids[1]));
    object* cube = make_cube_polygon(program_ids[1]);
    cube->scale(glm::vec3(10, 10, 10));
    cube->translate(glm::vec3(0, 0, 20));
    scene2.add_object(cube);

    fps_manager fps_manager;
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        const devices_state &device = devices_state::get_instance(window);

        /* Swap front and back buffers */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program_ids[0]);
        scene1.update_and_draw(device);

        glUseProgram(program_ids[1]);
        // update and draw scene2
        //scene2.update_and_draw(device);
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        GLenum error = glGetError();
        if (error)
            std::cerr << gluErrorString(error) << std::endl;
        //fps_manager.update_and_print_fps();
        fps_manager.update_and_set_window_title(window, "OpenGL");
    }

    glfwTerminate();
    glDeleteProgram(program_ids[0]);
    return 0;
}
