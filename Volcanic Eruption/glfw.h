#ifndef GLFW_H
#define GLFW_H

#include <glew.h>
#define GLFW_DLL
#include <glfw3.h>
#include "glm_camera.h"

#include "basic_glfw_camera.h"

GLFWwindow* glfw_init(int major_ver, int minor_ver, int width, int height, bool is_full_screen, const char* title);

bool GLFWCameraHandler(CameraMovement& Movement, int Key, int Action, int Mods);

#endif