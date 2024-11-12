#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    GLuint ID;

    // Default constructor that loads default shaders
    Shader();

    // Constructor with custom shader paths
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    void Use() const;
    void SetUniformMatrix4fv(const std::string& name, const glm::mat4& mat) const;
    void SetUniform1i(const std::string& name, int value) const;

private:
    void InitShader(const std::string& vertexPath, const std::string& fragmentPath);
    void CheckCompileErrors(GLuint shader, const std::string& type) const;
};
