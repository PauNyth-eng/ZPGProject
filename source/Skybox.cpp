//
// Created by Pavel Janu(Skola) on 11/25/2023.
//

#include "Skybox.h"


Skybox::Skybox(std::shared_ptr<Texture> cubeMap) : cubeMap(std::move(cubeMap)), shader(ShaderManager::skybox()) {
    initVbo();
    initVao();
}

void Skybox::initVao() {


    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

    using type = decltype(faces)::value_type;
    constexpr int typeSize = sizeof(type);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*typeSize, nullptr);
}

void Skybox::initVbo()
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    constexpr int typeSize = sizeof(decltype(faces)::value_type);
    glBufferData(GL_ARRAY_BUFFER, faces.size() * typeSize, faces.data(), GL_STATIC_DRAW);
}

void Skybox::draw() const {
    glDepthMask(GL_FALSE);
    shader.use();
    cubeMap->bind(shader);
    glStencilFunc(GL_ALWAYS, -1, 0xFF);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, faces.size());
    glDepthMask(GL_TRUE);
}


const std::vector<float> Skybox::faces = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
};

