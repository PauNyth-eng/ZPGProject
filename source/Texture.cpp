//
// Created by fpeterek on 29.11.20.
//

#include "Texture.h"

#include <cstdint>

#include <SOIL/SOIL.h>


static GLuint loadTexture(const std::string & texturePath) {
    return SOIL_load_OGL_texture(texturePath.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
}

Texture::Texture(const GLuint id, std::string texPath) : textureId(id), texturePath(std::move(texPath)) { }

Texture::Texture(const std::string & texturePath) : Texture(loadTexture(texturePath), texturePath) { }

void Texture::bind(Shader & shader) const {
    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glBindTexture(GL_TEXTURE_2D, textureId);
    shader.passUniformLocation("textureUnitID", std::int32_t(0));
}


TextureNotFoundException::TextureNotFoundException(const std::string & msg) noexcept : std::runtime_error(msg) { }
TextureAlreadyExistsException::TextureAlreadyExistsException(const std::string & msg) noexcept : std::runtime_error(msg) { }


std::unordered_map<std::string, std::shared_ptr<Texture>> TextureManager::map;

std::shared_ptr<Texture> TextureManager::get(const std::string & texture) {
    auto iter = map.find(texture);

    if (iter != map.end()) {
        return iter->second;
    }

    throw TextureNotFoundException("No such texture found: '" + texture + "'");
}

std::shared_ptr<Texture> TextureManager::emplace(const std::string & texture, const std::string & texturePath) {
    auto tex = std::make_shared<Texture>(texturePath);

    auto retval = map.emplace(texture, tex);
    if (not retval.second) {
        throw TextureAlreadyExistsException("Texture '" + texture + "' already exists.");
    }

    return retval.first->second;
}

std::shared_ptr<Texture> TextureManager::getOrEmplace(const std::string & texture, const std::string & texturePath) {

    auto iter = map.find(texture);

    if (iter != map.end()) {
        return iter->second;
    }

    auto tex = std::make_shared<Texture>(texturePath);
    return map.emplace(texture, tex).first->second;
}

std::shared_ptr<Texture> TextureManager::cubeMap(const std::string & cubemap, const std::vector<std::string> & textures) {

    auto iter = map.find(cubemap);

    if (iter != map.end()) {
        return iter->second;
    }

    auto cm = std::make_shared<CubeMap>(textures);
    return map.emplace(cubemap, cm).first->second;
}

static GLuint loadCubemap(const std::vector<std::string> & textures) {
    return SOIL_load_OGL_cubemap(
            textures[0].c_str(), textures[1].c_str(), textures[2].c_str(),
            textures[3].c_str(), textures[4].c_str(), textures[5].c_str(),
            SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, 0
    );
}

CubeMap::CubeMap(const std::vector<std::string> & textures) : Texture(loadCubemap(textures), textures[0]) { }

void CubeMap::bind(Shader & shader) const {

    Texture::bind(shader);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


}