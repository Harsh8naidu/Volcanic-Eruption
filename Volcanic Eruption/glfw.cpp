//#ifdef WIN32
//#include <Windows.h>
//#endif
//#include <stdio.h>
//
//#include "util.h"
//#include "glfw.h"
//#include "basic_glfw_camera.h"
//
//#include <iostream>
//
//static int glMajorVersion = 0;
//static int glMinorVersion = 0;
//
//int GetGLMajorVersion()
//{
//    return glMajorVersion;
//}
//
//int GetGLMinorVersion()
//{
//    return glMinorVersion;
//}
//
//int IsGLVersionHigher(int MajorVer, int MinorVer)
//{
//    return ((glMajorVersion >= MajorVer) && (glMinorVersion >= MinorVer));
//}
//
//
//static void glfw_lib_init()
//{
//    if (glfwInit() != 1) {
//		std::cout << "Failed to initialize GLFW" << std::endl;
//        exit(1);
//    }
//
//    int Major, Minor, Rev;
//
//    glfwGetVersion(&Major, &Minor, &Rev);
//
//    printf("GLFW %d.%d.%d initialized\n", Major, Minor, Rev);
//}
//
//
//static void enable_debug_output()
//{
//    glEnable(GL_DEBUG_OUTPUT);
//    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//    glDebugMessageCallback(glDebugOutput, nullptr);
//    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
//}
//
//
//// Must be done after glfw is initialized!
//static void init_glew()
//{
//    glewExperimental = GL_TRUE;
//    GLenum res = glewInit();
//    if (res != GLEW_OK) {
//        std::cout << "Failed to initialize GLFW" << std::endl;
//        exit(1);
//    }
//}
//
//
//GLFWwindow* glfw_init(int major_ver, int minor_ver, int width, int height, bool is_full_screen, const char* title)
//{
//    glfw_lib_init();
//
//    GLFWmonitor* monitor = is_full_screen ? glfwGetPrimaryMonitor() : NULL;
//
//    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
//
//    if (major_ver > 0) {
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_ver);
//    }
//
//    if (minor_ver > 0) {
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_ver);
//    }
//
//    GLFWwindow* window = glfwCreateWindow(width, height, title, monitor, NULL);
//
//    if (!window) {
//        const char* pDesc = NULL;
//        int error_code = glfwGetError(&pDesc);
//
//		std::cout << "Failed to create window: " << pDesc << std::endl;
//        exit(1);
//    }
//
//    glfwMakeContextCurrent(window);
//
//    // The following functions must be called after the context is made current
//    glGetIntegerv(GL_MAJOR_VERSION, &glMajorVersion);
//    glGetIntegerv(GL_MINOR_VERSION, &glMinorVersion);
//
//    if (major_ver > 0) {
//        if (major_ver != glMajorVersion) {
//			std::cout << "Requested major version " << major_ver << " is not the same as created version " << glMajorVersion << std::endl;
//            exit(0);
//        }
//    }
//
//    if (minor_ver > 0) {
//        if (minor_ver != glMinorVersion) {
//			std::cout << "Requested minor version " << minor_ver << " is not the same as created version " << glMinorVersion << std::endl;
//            exit(0);
//        }
//    }
//
//    // Must be done after glfw is initialized!
//    init_glew();
//
//    enable_debug_output();
//
//    glfwSwapInterval(1);
//
//    return window;
//}
//
//
//bool GLFWCameraHandler(CameraMovement& Movement, int Key, int Action, int Mods)
//{
//    bool Press = Action != GLFW_RELEASE;
//
//    bool Handled = true;
//
//    switch (Key) {
//
//    case GLFW_KEY_W:
//        Movement.Forward = Press;
//        break;
//
//    case GLFW_KEY_S:
//        Movement.Backward = Press;
//        break;
//
//    case GLFW_KEY_A:
//        Movement.StrafeLeft = Press;
//        break;
//
//    case GLFW_KEY_D:
//        Movement.StrafeRight = Press;
//        break;
//
//    case GLFW_KEY_PAGE_UP:
//        Movement.Up = Press;
//        break;
//
//    case GLFW_KEY_PAGE_DOWN:
//        Movement.Down = Press;
//        break;
//
//    default:
//        Handled = false;
//    }
//
//    if (Mods & GLFW_MOD_SHIFT) {
//        Movement.FastSpeed = Press;
//    }
//
//    return Handled;
//}