#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "basic_glfw_camera.h"
#include "shader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class ModelLoader {
public:
    static void LoadModel(const std::string& path);
    void RenderModel(const BasicCamera& camera);
    std::vector<Vertex>& GetVertices(); // Getter for vertices

private:
    static void ProcessNode(aiNode* node, const aiScene* scene);
    static void ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Vertex> vertices;  // Store model vertices here
    GLuint m_modelVAO;
    GLuint m_modelVBO;
    GLuint m_texture;
    GLuint m_numVertices;
    Shader m_shader;
};