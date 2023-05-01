#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Core/Simulator.hpp"
#include "Render/ResourceManager.hpp"

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_callback(GLFWwindow* window, double xPos, double yPos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mode);

// The Width of the screen
const unsigned int SCREEN_WIDTH = 1000;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 1000;

Core::Simulator GameOfLife(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GameOfLife", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialize game
    // ---------------
    GameOfLife.Init();

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float fpsCounterTimer = 0;
    int frameCounter = 0;

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        frameCounter++;
        fpsCounterTimer += deltaTime;
        if(fpsCounterTimer >= 0.1)
        {
            float fps = (1.0f / fpsCounterTimer) * frameCounter;
            float msPreFrame = (fpsCounterTimer / frameCounter) * 1000;
            std::string newTitle = "GameOfLife - " + std::to_string(fps) + "FPS / " + std::to_string(msPreFrame) + "ms";
            glfwSetWindowTitle(window, newTitle.c_str());
            frameCounter = 0;
            fpsCounterTimer = 0;
        }

        glfwPollEvents();

        // manage user input
        // -----------------
        GameOfLife.ProcessInput(deltaTime);

        // update game state
        // -----------------
        GameOfLife.Update(deltaTime);

        // render
        // ------
        if(GameOfLife.State == Core::SIMULATION_PAUSED)
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        else
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        GameOfLife.Render();

        glfwSwapBuffers(window);
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    Render::ResourceManager::Clear();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            GameOfLife.Keys[key] = KEY_PRESSED;
        else if (action == GLFW_RELEASE)
            GameOfLife.Keys[key] = KEY_RELEASED;
    }
}

void cursor_callback(GLFWwindow* window, double xPos, double yPos)
{
    GameOfLife.cursorWindowX = glm::clamp(xPos, 0.0, static_cast<double>(SCREEN_WIDTH));
    GameOfLife.cursorWindowY = glm::clamp(yPos, 0.0, static_cast<double>(SCREEN_HEIGHT));
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mode)
{
    if(button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
            GameOfLife.rightMouseButton = KEY_PRESSED;
        else if (action == GLFW_RELEASE)
            GameOfLife.rightMouseButton = KEY_RELEASED;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
            GameOfLife.leftMouseButton = KEY_PRESSED;
        else if (action == GLFW_RELEASE)
            GameOfLife.leftMouseButton= KEY_RELEASED;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}