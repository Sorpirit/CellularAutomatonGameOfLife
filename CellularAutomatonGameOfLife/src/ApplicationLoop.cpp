#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib>

#include <Core/SimulatorState.h>
#include <GameOfLife/GameOfLifeManager.hpp>
#include <Render/ResourceManager.hpp>
//#define TESTING_DETERMINISTIC

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_callback(GLFWwindow* window, double xPos, double yPos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mode);
bool contains(int argc, char** argv, const std::string& value);
int indexOf(int argc, char** argv, const std::string& value);
bool tryParseInt(char* str, int& result);

const unsigned int SCREEN_WIDTH = 1000;
const unsigned int SCREEN_HEIGHT = 1000;

GameOfLife::GameOfLifeManager GameManager(SCREEN_WIDTH, SCREEN_HEIGHT);

bool hasRunOption(int argc, char** argv)
{
    return std::find(argv, argv + argc, std::string("-frameCounter")) != argv + argc;
}

int main(int argc, char** argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
    GameManager.Init();

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    //tick limiter
    int timerArgIndex = indexOf(argc, argv, "-tickLimiter");
    bool tickLimiter = false;
    float minDeltaTime = 0.05f;
    if(timerArgIndex != -1)
    {
        tickLimiter = true;
        int minDeltaTimeMs = 200;
        if(!tryParseInt(argv[timerArgIndex + 1], minDeltaTimeMs))
        {
            minDeltaTimeMs = 200;
        }
        minDeltaTime = minDeltaTimeMs / 1000.0f;
    }

    //fps counter
    bool frameCounterEnable = contains(argc, argv, "-frameCounter");
    float fpsCounterTimer = 0;
    int frameCounter = 0;

#ifdef TESTING_DETERMINISTIC
    int testFrameCounter = 1000;
    frameCounterEnable = false;
    tickLimiter = false;
    GameManager.State = Core::GameState::SIMULATION_RUNNING;
#endif


    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if(tickLimiter && deltaTime < minDeltaTime)
        {
            int targetMs = static_cast<int>(1000.0f * (minDeltaTime - deltaTime));
            std::this_thread::sleep_for(std::chrono::milliseconds(targetMs));
            lastFrame = static_cast<float>(glfwGetTime());
            deltaTime = minDeltaTime;
        }

        if(frameCounterEnable)
        {
            frameCounter++;
            fpsCounterTimer += deltaTime;
            if (fpsCounterTimer >= 0.1)
            {
                float fps = (1.0f / fpsCounterTimer) * frameCounter;
                float msPreFrame = (fpsCounterTimer / frameCounter) * 1000;
                std::string newTitle = "GameOfLife - " + std::to_string(fps) + "FPS / " + std::to_string(msPreFrame) + "ms";
                glfwSetWindowTitle(window, newTitle.c_str());
                frameCounter = 0;
                fpsCounterTimer = 0;
            }
        }

        glfwPollEvents();

        // manage user input
        // -----------------
        GameManager.ProcessInput(deltaTime);

        // update game state
        // -----------------
        GameManager.Update(deltaTime);

        // render
        // ------
        if(GameManager.State == Core::SIMULATION_PAUSED)
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        else
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        GameManager.Render();

        glfwSwapBuffers(window);

#ifdef TESTING_DETERMINISTIC
        testFrameCounter--;
        if(testFrameCounter == 0)
			break;
#endif
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
            GameManager.Keys[key] = KEY_PRESSED;
        else if (action == GLFW_RELEASE)
            GameManager.Keys[key] = KEY_RELEASED;
    }
}

void cursor_callback(GLFWwindow* window, double xPos, double yPos)
{
    GameManager.CursorWindowX = glm::clamp(xPos, 0.0, static_cast<double>(SCREEN_WIDTH));
    GameManager.CursorWindowY = glm::clamp(yPos, 0.0, static_cast<double>(SCREEN_HEIGHT));
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mode)
{
    if(button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
            GameManager.RightMouseButton = KEY_PRESSED;
        else if (action == GLFW_RELEASE)
            GameManager.RightMouseButton = KEY_RELEASED;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
            GameManager.LeftMouseButton = KEY_PRESSED;
        else if (action == GLFW_RELEASE)
            GameManager.LeftMouseButton= KEY_RELEASED;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

bool contains(int size, char** arr, const std::string& value)
{
    return std::find(arr, arr + size, value) != arr + size;
}


int indexOf(int size, char** arr, const std::string& value)
{
    auto it = std::find(arr, arr + size, value);
    if (it != arr + size) {
        return std::distance(arr, it);
    }

    return -1;
}

bool tryParseInt(char* str, int& result)
{
    std::istringstream iss(str);
    iss >> result;
    bool fail = iss.fail();
    bool eof = iss.eof();
    return !iss.fail() && iss.eof();
}