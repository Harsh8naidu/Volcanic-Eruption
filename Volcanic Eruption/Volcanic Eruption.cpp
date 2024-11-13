#include <stdio.h>
#include <string.h>
#include <math.h>
#include <glad/glad.h>
#include "util.h"
#include "basic_glfw_camera.h"
#include "terrain.h"
#include <glfw3.h>
#include "model_loader.h"
#include "texture_loader.h"
#include <iostream>

// Constants for window size
const GLuint WINDOW_WIDTH = 800;
const GLuint WINDOW_HEIGHT = 600;

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void CursorPosCallback(GLFWwindow* window, double x, double y);
static void MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode);

class VolcanicEruption
{
public:
    VolcanicEruption() : m_pGameCamera(nullptr), m_isWireframe(false), m_shader("terrain.vs", "terrain.fs")
    {
        // Create ModelLoader object
        m_modelLoader = std::make_unique<ModelLoader>();

        // Initialize the Terrain object with the ModelLoader pointer
        m_terrain = Terrain(m_modelLoader.get());
    }

    virtual ~VolcanicEruption()
    {
        delete m_pGameCamera;
    }

    void Init()
    {
        CreateWindow();
        InitCallbacks();
        InitCamera();
        InitTerrain();
    }

    void Run()
    {
        while (!glfwWindowShouldClose(window)) {
            RenderScene();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void PassiveMouseCB(int x, int y)
    {
        m_pGameCamera->OnMouse(x, y);
    }

    void KeyboardCB(int key, int state)
    {
        if (state == GLFW_PRESS) {
            switch (key) {
            case GLFW_KEY_ESCAPE:
            case GLFW_KEY_Q:
                glfwDestroyWindow(window);
                glfwTerminate();
                exit(0);

            case GLFW_KEY_C:
                m_pGameCamera->Print();
                break;

            case GLFW_KEY_W:
                m_isWireframe = !m_isWireframe;
                if (m_isWireframe) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                break;
            }
        }

        m_pGameCamera->OnKeyboard(key);
    }

    void MouseCB(int button, int action, int x, int y)
    {
    }

private:
    void CreateWindow() {
        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }

        // Set GLFW window properties for OpenGL version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Use OpenGL 4.x
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Use OpenGL 4.5 (adjust if needed)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile

        // Create GLFW window
        window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Volcanic Eruption", nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        // Set the current OpenGL context
        glfwMakeContextCurrent(window);

        if (!glfwGetCurrentContext()) {
            std::cerr << "Failed to set the OpenGL context" << std::endl;
            glfwDestroyWindow(window);
            glfwTerminate();
            return;
        }

        // Load OpenGL functions with GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            glfwDestroyWindow(window);
            glfwTerminate();
            return;
        }

        // Set OpenGL viewport and window user pointer
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        glfwSetWindowUserPointer(window, this); // Set user pointer to the `VolcanicEruption` instance
    }

    void InitCallbacks()
    {
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetCursorPosCallback(window, CursorPosCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
    }

    void InitCamera()
    {
        Vector3f Pos(100.0f, 220.0f, -400.0f);
        Vector3f Target(0.0f, -0.25f, 1.0f);
        Vector3f Up(0.0f, 1.0f, 0.0f);

        float FOV = 45.0f;
        float zNear = 0.1f;
        float zFar = 2000.0f;
        float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
        PersProjInfo persProjInfo = { FOV, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, zNear, zFar };

        m_pGameCamera = new BasicCamera(persProjInfo, Pos, Target, Up);
    }

    void InitTerrain()
    {
        // Load the volcano model (OBJ file)
        m_modelLoader->LoadModel("Assets/Models/heightmap_lowres_v03.obj");

        // Load the textures
        GLuint colormap = LoadTexture("colormap_v03.png");
        GLuint normalMap = LoadTexture("normalmap_v03.png");

        // Apply textures in your shaders
        m_shader.Use();
        glUniform1i(glGetUniformLocation(m_shader.ID, "colormap"), 0);  // Texture unit 0
        glUniform1i(glGetUniformLocation(m_shader.ID, "normalMap"), 1);  // Texture unit 1

        // Load the heightmap
        m_terrain.LoadHeightmap("heightmap.tif", 512, 512);
    }

    void RenderScene()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_terrain.Render(*m_pGameCamera);
    }

private:
    GLFWwindow* window;
    BasicCamera* m_pGameCamera;
    bool m_isWireframe;
    std::unique_ptr<ModelLoader> m_modelLoader;  // Smart pointer to manage ModelLoader
    Terrain m_terrain;
    Shader m_shader;
};

VolcanicEruption* app = nullptr;

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    VolcanicEruption* app = static_cast<VolcanicEruption*>(glfwGetWindowUserPointer(window)); // Get the `VolcanicEruption` instance
    app->KeyboardCB(key, action);
}

static void CursorPosCallback(GLFWwindow* window, double x, double y)
{
    VolcanicEruption* app = static_cast<VolcanicEruption*>(glfwGetWindowUserPointer(window)); // Get the `VolcanicEruption` instance
    app->PassiveMouseCB((int)x, (int)y);
}

static void MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode)
{
    VolcanicEruption* app = static_cast<VolcanicEruption*>(glfwGetWindowUserPointer(window)); // Get the `VolcanicEruption` instance
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    app->MouseCB(Button, Action, (int)x, (int)y);
}

int main(int argc, char** argv)
{
    app = new VolcanicEruption();
    app->Init();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    app->Run();

    delete app;
    return 0;
}