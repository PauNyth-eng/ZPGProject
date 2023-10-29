//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#include "ShaderManager.h"


ShaderManager& ShaderManager::instance()
{
    if (not sm) {
        sm = new ShaderManager();
    }
    return *sm;
}

ShaderManager* ShaderManager::sm = nullptr;


Shader& ShaderManager::constant()
{
    return instance()._constant;
}

Shader& ShaderManager::lambert()
{
    return instance()._lambert;
}

Shader& ShaderManager::phong()
{
    return instance()._phong;
}

Shader& ShaderManager::phong2()
{
    return instance()._phong2;
}

Shader& ShaderManager::blinn()
{
    return instance()._blinn;
}


ShaderManager::ShaderManager() : _constant("../resources/Shaders/Vertex.vert", "../resources/Shaders/Constant.frag"),
                                    _lambert("../resources/Shaders/Vertex.vert", "../resources/Shaders/Lambert.frag"),
                                    _phong("../resources/Shaders/Vertex.vert", "../resources/Shaders/Phong.frag"),
                                    _phong2("../resources/Shaders/Vertex.vert", "../resources/Shaders/Phong2.frag"),
                                    _blinn("../resources/Shaders/Vertex.vert", "../resources/Shaders/Blinn.frag")
{

}
