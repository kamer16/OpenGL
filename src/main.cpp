#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
#include <glm/gtc/type_ptr.hpp> /* value_ptr */

#include "shader.hpp" // loadShaders()
#include "texture.hpp" // loadTextures()
#include "vbo.hpp" // BindArrays()
#include "render_scene.hpp" // renderScene()
#include "polygon.hpp" // polygons

#include "obj_loader.hpp"

#include <iostream>

void enableEnv();

void enableEnv()
{
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char *argv[])
{
    (void) argv;
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWmonitor *monitor = argc > 1 ? glfwGetPrimaryMonitor() : NULL;
    int width = 880;
    int height = 520;
    float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    window = glfwCreateWindow(width, height, "Hello World", monitor, NULL);
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

    GLuint vao_ids[16];
    BindArrays(program_ids[0], vao_ids);
    loadTextures(program_ids[0]);
    polygon *coord = polygon::coordinate_new(program_ids[1]);
    polygon *ground = polygon::quad_xz_new(program_ids[1]);

    std::vector<utility::vec3> vertices;
    std::vector<utility::vec3> normals;
    std::vector<utility::vec2> text_coords;
    load_obj("data/teapot/teapot.obj", vertices, normals, text_coords);
    GLuint mesh_vao_id;
    bind_object(program_ids[0], &mesh_vao_id, vertices, normals, text_coords);
    if (monitor)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else // TODO somehow not working, Bug in GLFW ??
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    /* Loop until the user closes the window */
    scene scene1(program_ids[0], aspect_ratio, window);
    scene scene2(program_ids[1], aspect_ratio, window);
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */

        /* Swap front and back buffers */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program_ids[0]);
        scene1.update();
        scene1.render_elements(vao_ids[0], 36);
        scene1.render_arrays(mesh_vao_id, static_cast<int>(vertices.size()));

        glUseProgram(program_ids[1]);
        scene2.update();
        coord->draw();
        ground->draw();
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        GLenum error = glGetError();
        if (error)
            std::cerr << gluErrorString(error) << std::endl;
    }

    glfwTerminate();
    return 0;
}
