#include "model_loader.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>

void ModelLoader::LoadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

void ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    // Extract mesh data (vertices, normals, texture coords)
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertices.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
        normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));

        if (mesh->mTextureCoords[0]) {
            texCoords.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
        }
    }
}

    // You can now use vertices, normals, and texCoords to render your model
void ModelLoader::RenderModel(const BasicCamera & camera)
{
    m_shader.Use();

    glm::mat4 model = glm::mat4(1.0f); // Apply any transformations if necessary
    Matrix4f view = camera.GetViewMatrix();
    Matrix4f projection = camera.GetProjectionMat();

    glUniformMatrix4fv(glGetUniformLocation(m_shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(m_shader.ID, "view"), 1, GL_FALSE, view.ToFloatPtr());
    glUniformMatrix4fv(glGetUniformLocation(m_shader.ID, "projection"), 1, GL_FALSE, projection.ToFloatPtr());

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // Render the model
    glBindVertexArray(m_modelVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
    glBindVertexArray(0);
}

std::vector<Vertex>& ModelLoader::GetVertices() {
    return vertices;
}
