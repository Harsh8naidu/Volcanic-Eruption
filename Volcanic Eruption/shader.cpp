#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>


// Default constructor that loads default shaders
Shader::Shader() {
    // Default shader paths
    std::string defaultVertexShader = "Assets/Shaders/terrain.vs";
    std::string defaultFragmentShader = "Assets/Shaders/terrain.fs";
    InitShader(defaultVertexShader, defaultFragmentShader);
}

// Constructor with custom shader paths
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    InitShader(vertexPath, fragmentPath);
}

void Shader::InitShader(const std::string& vertexPath, const std::string& fragmentPath) {
    // 1. Retrieve the vertex/fragment source code from file
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile(vertexPath);
    std::ifstream fShaderFile(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    // 2. Compile shaders
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(vertex);
    glCompileShader(fragment);

    // 3. Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // 4. Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

// Helper function to initialize shaders
//void Shader::InitShader(const std::string& vertexPath, const std::string& fragmentPath) {
//    // 1. Retrieve the vertex/fragment source code from file
//    std::string vertexCode;
//    std::string fragmentCode;
//    std::ifstream vShaderFile(vertexPath);
//    std::ifstream fShaderFile(fragmentPath);
//    std::stringstream vShaderStream, fShaderStream;
//
//    vShaderStream << vShaderFile.rdbuf();
//    fShaderStream << fShaderFile.rdbuf();
//
//    vertexCode = vShaderStream.str();
//    fragmentCode = fShaderStream.str();
//
//    // 2. Compile shaders
//    const char* vShaderCode = vertexCode.c_str();
//    const char* fShaderCode = fragmentCode.c_str();
//
//    GLuint vertex, fragment;
//    GLint success;
//    GLchar infoLog[512];
//
//    // Vertex Shader
//	if (glCreateShader) {
//		std::cerr << "ERROR::SHADER::VERTEX::FAILED_TO_CREATE_SHADER" << std::endl;
//		std::cout << "gl create shader failed to load" << std::endl;
//	}
//    vertex = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertex, 1, &vShaderCode, nullptr);
//    glCompileShader(vertex);
//    CheckCompileErrors(vertex, "VERTEX");
//
//    // Fragment Shader
//    fragment = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragment, 1, &fShaderCode, nullptr);
//    glCompileShader(fragment);
//    CheckCompileErrors(fragment, "FRAGMENT");
//
//    // Shader Program
//    ID = glCreateProgram();
//    glAttachShader(ID, vertex);
//    glAttachShader(ID, fragment);
//    glLinkProgram(ID);
//    CheckCompileErrors(ID, "PROGRAM");
//
//    // Delete the shaders as they are now linked into our program
//    glDeleteShader(vertex);
//    glDeleteShader(fragment);
//}

void Shader::Use() const {
    glUseProgram(ID);
}

void Shader::SetUniformMatrix4fv(const std::string& name, const glm::mat4& mat) const {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniform1i(const std::string& name, int value) const {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform1i(location, value);
}

void Shader::CheckCompileErrors(GLuint shader, const std::string& type) const {
    GLint success;
    GLchar infoLog[512];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    }
}
