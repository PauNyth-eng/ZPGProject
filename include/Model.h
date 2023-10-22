//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#pragma once
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <vector>
#include <filesystem>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace fs = std::filesystem;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 TexCoords;
};

struct Material {
    glm::vec3 diffuse{ 0.f };
    glm::vec3 specular{ 0.f };
    glm::vec3 ambient{ 0.f };
    std::string diffuseMap;
    std::string specularMap;
    std::string heightMap;
    float shininess = 0.f;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};
class Mesh {
public:
    // mesh data
    void Draw(Shader& shader);


    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::shared_ptr<Texture> texture, const Material& material);
    void applyTexture(std::shared_ptr<Texture> texture);
private:
    //  render data
    unsigned int VAO, VBO, EBO;
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::shared_ptr<Texture> texture;
    const Material& material;
    void setupMesh();
};


class Model {
public:

    void draw(uint32_t id, Shader& shader) const;
    void applyTexture(const std::shared_ptr<Texture>& texture);
    void AddObj(Mesh&& mesh);
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    std::string directory;

private:



};


class ModelLoader {

    static std::unordered_map<std::string, std::shared_ptr<Model>> models;

    static std::shared_ptr<Model> loadModel(const std::string& model);

    static constexpr uint32_t importOpts = aiProcess_OptimizeMeshes
                                           bitor aiProcess_JoinIdenticalVertices
                                           bitor aiProcess_Triangulate
                                           bitor aiProcess_CalcTangentSpace
                                           // bitor aiProcess_FlipUVs
                                           bitor aiProcess_GenSmoothNormals;

    static std::string formatName(const std::string& model);

    static void processNode(aiNode& node, const aiScene& scene, std::shared_ptr<Model>& model);
    static Mesh processMesh(aiMesh& mesh, const aiScene& scene, const Model& model);

    static void processMaterials(const aiScene& scene, std::vector<Material>& materials);
    static Material loadMaterial(aiMaterial& mat, Material& material);
    static glm::vec3 toVector(const aiColor3D& color);
    static void processIndices(aiMesh& mesh, std::vector<uint32_t>& indices);
    static void processVertices(aiMesh& mesh, std::vector<Vertex>& vertices);
    static std::string dewindows(std::string str);
    static glm::vec3 extractVector(aiVector3D& vec);
    static std::string lufthansa(std::string str);
public:

    static std::shared_ptr<Model> get(const std::string& model);
};


