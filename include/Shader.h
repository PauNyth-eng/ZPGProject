//
// Created by Pavel Janu(Skola) on 10/20/2023.
//

#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include "Observer.h"
#include "Camera.h"
#include "Light.h"
class Shader : public Observer{
public:
    Shader(const char* vertexPath, const char* fragmentPath);

    void use() const;
    void unUse() const;
    ~Shader();
    void updateView(const glm::mat4& view) const;
    void updateProjection(const glm::mat4& projection) const;
    void updatePosition(const glm::vec3& position) const;

    void passUniformLocation(const std::string& var, const glm::mat4& matrix) const;
    void passUniformLocation(const char* var, const glm::mat4& matrix) const;
    void passUniformLocation(const std::string& var, const glm::mat3& matrix) const;
    void passUniformLocation(const char* var, const glm::mat3& matrix) const;
    void passUniformLocation(const std::string& var, const glm::vec3& vector) const;
    void passUniformLocation(const char* var, const glm::vec3& vector) const;
    void passUniformLocation(const std::string& var, int32_t value) const;
    void passUniformLocation(const char* var, int32_t value) const;
    void passUniformLocation(const std::string& var, float value) const;
    void passUniformLocation(const char* var, float value) const;
    void notify(EventType eventType, void* object) override;
private:
    [[nodiscard]] bool checkShader() const;
    typedef decltype(GL_FRAGMENT_SHADER) ShaderType;
    GLuint vertexId;
    GLuint fragmentId;
    GLuint shaderId;
    [[nodiscard]] GLint getUniformLocation(const std::string& var) const;
    GLint getUniformLocation(const char* var) const;
    void compile(const std::string& vertexPath, const std::string& fragmentPath);
    static std::string loadShader(const std::string& filename);
    static GLuint compileShader(const std::string& shader, ShaderType type);

    void colorChanged(glm::vec3 color, size_t lightIndex, LightType lightType) const;
    void positionChanged(glm::vec3 position, size_t lightIndex, LightType lightType);
    void typeChanged(LightType type, size_t lightIndex);

    void applyLight(Light & light);
    void applyLight(PositionedLight & light);
    void applyLight(DirectionalLight & light);
    void applyLight(SpotLight & light);

};

