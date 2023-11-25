//
// Created by Pavel Janu(Skola) on 10/20/2023.
//

#pragma once

#include <vector>
#include <functional>
#include <optional>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include "TransComponent.h"
#include "TransComposite.h"
#include "TransRotate.h"
#include "TransTranslate.h"
#include "TransScale.h"
#include "Model.h"
#include "Enums.h"
#include "Shader.h"
#include "Texture.h"

class Object
{
private:
    glm::vec3 color = defaultColor;



    [[nodiscard]] glm::mat4 transformation() const;
    std::reference_wrapper<Shader> shader;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Model> model;
    unsigned int id;


    static unsigned int objectCount;
    static unsigned int getNextId();


    Scale scaleDr = Scale::none;
    Rotation rotateDr = Rotation::left;
    glm::vec2 movementDir { 0.f, 0.f };
    glm::vec2 forces { 0.f, 0.f };
    glm::vec3 rotationRadians{ 0.f };
    glm::vec3 rotationCenter{ 0.f };
    glm::vec3 direction{ 0.f };
    glm::vec3 position{ 0.f };
    float radius = 0.0f;


public:
    Object() = delete;
    Object(std::shared_ptr<Model> model, Shader& shader, std::shared_ptr<Texture> texture);
    void draw() const;
    void update(double dt);
    void AddTransformation(TransComponent* component);
    [[nodiscard]] unsigned int objectId() const;

    std::shared_ptr<TransComposite> composite;

    void updateScale(float d);
    void updateRotate(float d);
    void updateMove(float d);

    void setScaleDr(Scale scaleDr);
    void setRotateDr(Rotation rotateDr);


    void setRotate(glm::vec3 radians, glm::vec3 direction, glm::vec3 rotationCenter, float radius = 0.0f);
    void setScale(glm::vec3 scales);
    void setMove(glm::vec3 position);



    void setColor(glm::vec3 color);
    void setColor(float r, float g, float b);

    static const glm::vec3 defaultColor;
    static const glm::vec3 secondaryColor;


    class Builder
    {
        std::shared_ptr<Model> model;
        Shader* shader = nullptr;
        std::shared_ptr<Texture> texture;


        glm::vec3 rotationRadians{ 0.f };
        glm::vec3 rotationCenter{ 0.f };
        glm::vec3 direction{ 0.f };
        glm::vec3 position{ 0.f };
        glm::vec3 scales{ 1.f };
        float radius = 0.0f;
        glm::vec3 color = defaultColor;
        void reset();
        Object createObject();

    public:
        Builder& setModel(std::shared_ptr<Model> model);
        Builder& setShader(Shader& shader);
        Builder & setTexture(std::shared_ptr<Texture> texture);
        Builder& emplaceObject(std::shared_ptr<Model> model, Shader& shader, std::shared_ptr<Texture> texture);
        Builder& setRotation(glm::vec3 radians, glm::vec3 direction, glm::vec3 rotationCenter, float radius = 0.0f);
        Builder& setPosition(glm::vec3 position);
        Builder& setPosition(float x, float y, float z);
        Builder& setScale(glm::vec3 scales);
        Builder& setScale(float x, float y, float z);
        Builder& setColor(glm::vec3 color);
        Builder& setColor(float r, float g, float b);
        Object build();
    };

};


