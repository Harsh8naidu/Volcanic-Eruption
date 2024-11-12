#include "terrain.h"
#include <stb_image.h>
#include <iostream>

void Terrain::LoadHeightmap(const char* filename, int width, int height) {
    int imgWidth, imgHeight, channels;
    unsigned char* image = stbi_load(filename, &imgWidth, &imgHeight, &channels, 1);  // Load as grayscale

    if (!image) {
        std::cerr << "Error loading heightmap image" << std::endl;
        return;
    }

    std::vector<float> heightmapData(width * height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = (y * width) + x;
            heightmapData[idx] = static_cast<float>(image[idx]) / 255.0f; // Normalize to [0, 1]
        }
    }

    ApplyHeightmapToModel(heightmapData);  // Apply the heightmap data to the model

    stbi_image_free(image);
}

void Terrain::ApplyHeightmapToModel(const std::vector<float>& heightmapData) {
    auto& vertices = m_model->GetVertices();
    for (size_t i = 0; i < vertices.size() && i < heightmapData.size(); ++i) {
        vertices[i].position.y = heightmapData[i] * maxHeight;  // maxHeight is your height scale
    }
}

void Terrain::Render(const BasicCamera& camera)
{
    // Use the shader program
    m_shader.Use();

    // Get the camera's view and projection matrices
    Matrix4f view = camera.GetViewMatrix(); // Correct view matrix function
    Matrix4f projection = camera.GetProjectionMat(); // Correct projection matrix function

    // Optionally, if you want the combined view-projection matrix:
    // glm::mat4 viewProj = camera.GetViewProjMatrix(); // Get the combined view-projection matrix

    // Set up the model matrix (identity for now)
    glm::mat4 model = glm::mat4(1.0f); // You can apply transformations to this matrix later if needed

    // Pass the matrices to the shader
    glUniformMatrix4fv(glGetUniformLocation(m_shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(m_shader.ID, "view"), 1, GL_FALSE, view.ToFloatPtr());
    glUniformMatrix4fv(glGetUniformLocation(m_shader.ID, "projection"), 1, GL_FALSE, projection.ToFloatPtr());

    // Bind the textures (colormap, normal map)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalMap);

    // Render the terrain
    glBindVertexArray(m_terrainVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_width * m_height); // Assuming terrain is made of triangles
    glBindVertexArray(0); // Unbind VAO
}
