//
// Created by Pavel Janu(Skola) on 11/25/2023.
//

#pragma once

#include <memory>

#include "ShaderManager.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"


class Skybox {

    static const std::vector<float> faces;

    std::shared_ptr<Texture> cubeMap;
    Shader & shader;

    GLuint  vbo = 0;
    GLuint  vao = 0;

    void initVao();
    void initVbo();

public:
    Skybox() = delete;
    explicit Skybox(std::shared_ptr<Texture> cubeMap);
    void draw() const;
};


