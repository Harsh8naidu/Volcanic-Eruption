#pragma once
#include <vector>
#include "basic_glfw_camera.h"
#include "shader.h"
#include "model_loader.h" // Ensure this header is necessary

class Terrain {
public:
    // Default constructor
    Terrain() : m_model(nullptr), m_width(0), m_height(0), m_texture(0), m_normalMap(0), m_terrainVAO(0), m_shader("terrain.vs", "terrain.fs"), maxHeight(40.0f) {}

    // Constructor that accepts a ModelLoader pointer
    Terrain(ModelLoader* model) : m_model(model), m_width(0), m_height(0), m_texture(0), m_normalMap(0), m_terrainVAO(0), m_shader("terrain.vs", "terrain.fs"), maxHeight(40.0f) {}

    void LoadHeightmap(const char* filename, int width, int height);
    void ApplyHeightmapToModel(const std::vector<float>& heightmapData);
    void Render(const BasicCamera& camera);

private:
    GLuint m_width;
    GLuint m_height;
    GLuint m_texture;
    GLuint m_normalMap;
    GLuint m_terrainVAO;
    Shader m_shader;
    float maxHeight;

    ModelLoader* m_model;  // Pointer to ModelLoader object
};
