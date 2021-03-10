#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "learnopengl/shader_m.h"
#include "learnopengl/camera.h"

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

unsigned int createShader(const char* vsSrc, const char* fsSrc);
bool compileShader(const char* src, GLuint shader);
bool linkShaderProg(GLuint prog, GLuint vet_shader, GLuint pixel_shader);


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(800, 600, "test", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float vertices[9] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    const char* vsSrc = R"(
     #version 400 core
     layout (location = 0) in vec3 pos;
     void main()
     {
        gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
     })";

    const char* fsSrc = R"(
     #version 400 core
     out vec4 outColor;
     void main()
     {
         outColor = vec4(0.5f, 0.2f, 0.5f, 1.0f);
     })";

    GLuint shader = createShader(vsSrc, fsSrc);
    glUseProgram(shader);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(1.0, 1.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        //glDrawArrays(GL_LINES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


static unsigned int createShader(const char* vsSrc, const char* fsSrc)
{
    auto hVetShader = glCreateShader(GL_VERTEX_SHADER);
    if (compileShader(vsSrc, hVetShader) == false)
        return 0;

    auto hPixelShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (compileShader(fsSrc, hPixelShader) == false)
        return 0;

    auto hShaderProg = glCreateProgram();
    if (linkShaderProg(hShaderProg, hVetShader, hPixelShader) == false)
        return 0;

    glDeleteShader(hVetShader);
    glDeleteShader(hPixelShader);

    return hShaderProg;
}

static bool compileShader(const char* src, GLuint shader)
{
    char errMsg[512] = { 0 };
    int  err;
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &err);
    if (err == 0)
    {
        glGetShaderInfoLog(shader, 512, NULL, errMsg);
        std::cout << "×ÅÉ«Æ÷±àÒë´íÎó: " << errMsg << std::endl;;
        return false;
    }
    return true;
}

static bool linkShaderProg(GLuint prog, GLuint vet_shader, GLuint pixel_shader)
{
    char errMsg[512] = { 0 };
    int  err;
    glAttachShader(prog, vet_shader);
    glAttachShader(prog, pixel_shader);
    glLinkProgram(prog);
    glGetProgramiv(prog, GL_LINK_STATUS, &err);
    if (err == 0)
    {
        glGetProgramInfoLog(prog, 512, NULL, errMsg);
        std::cout << "×ÅÉ«Æ÷Á´½Ó´íÎó: " << errMsg << std::endl;;
        return false;
    }
    return true;
}