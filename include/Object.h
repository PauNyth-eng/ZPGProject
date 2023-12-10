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
#include "Movement.h"

class Object
{
private:
    glm::vec3 color = defaultColor;


    std::shared_ptr<TransTranslate> translateMat;
    std::shared_ptr<TransScale> scaleMat;
    std::shared_ptr<TransRotate> rotateMatX;
    std::shared_ptr<TransRotate> rotateMatY;
    std::shared_ptr<TransRotate> rotateMatZ;
    [[nodiscard]] glm::mat4 transformation() const;
    std::reference_wrapper<Shader> shader;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Model> model;
    std::optional<std::shared_ptr<MovementCalculator>> movementCalculator;
    unsigned int id;


    static unsigned int objectCount;
    static unsigned int getNextId();


    glm::vec3 translation { 0.f };
    glm::vec3 rotation { 0.f };
    glm::vec3 scales { 1.f };

    Scale scaleDr = Scale::none;
    Rotation rotateDr = Rotation::none;
    glm::vec2 movementDir { 0.f, 0.f };

    float rotationV = 0;
    glm::vec2 forces { 0.f, 0.f };

    static constexpr float growthV = 1.0;

    static constexpr float rotationAcc = 1.0;
    static constexpr float rotationDec = 0.5;
    static constexpr float rotationMax = 10.0;

    static constexpr float acceleration = 1.0;
    static constexpr float deceleration = 0.5;
    static constexpr float maxV = 10.0;

    static float getAcc(Direction dir);

    static float calcNewV(float current, float acc, float dec);
    static float capV(float current, float max);


    void updateGrowth(float dt);
    void updateRotation(float dt);
    void updateForces(float dt);

public:
    Object() = delete;
    Object(std::shared_ptr<Model> model, Shader& shader, std::shared_ptr<Texture> texture);
    Object(std::shared_ptr<Model> model, Shader& shader, std::shared_ptr<Texture> texture, std::shared_ptr<MovementCalculator> movementCalculator);
    void draw() const;
    void update(double dt);
    void AddTransformation(std::shared_ptr<TransComponent>  component);
    [[nodiscard]] unsigned int objectId() const;

    std::shared_ptr<TransComposite> composite = std::make_shared<TransComposite>();

    void rotate(glm::vec3 radians);
    void move(glm::vec3 delta);
    void scale(glm::vec3 scales);

    void applyForce(Direction dx, Direction dy);
    void applyFx(Direction dir);
    void applyFy(Direction dir);
    void enableGrowth(Scale growth);
    void disableGrowth();
    void enableRotation(Rotation rotation);
    void disableRotation();


    void updateMovement(double dt);




    void setColor(glm::vec3 color);
    void setColor(float r, float g, float b);

    static const glm::vec3 defaultColor;
    static const glm::vec3 secondaryColor;


    class Builder
    {
        std::shared_ptr<Model> model;
        Shader* shader = nullptr;
        std::shared_ptr<Texture> texture;
        std::optional<std::shared_ptr<MovementCalculator>> movementCalculator;

        glm::vec3 rotationRadians { 0.f };
        glm::vec3 position { 0.f };
        glm::vec3 scales { 1.f };
        glm::vec3 color = defaultColor;
        void reset();
        Object createObject();

    public:
        Builder& setModel(std::shared_ptr<Model> model);
        Builder& setShader(Shader& shader);
        Builder & setTexture(std::shared_ptr<Texture> texture);
        Builder& emplaceObject(std::shared_ptr<Model> model, Shader& shader, std::shared_ptr<Texture> texture);
        Builder& setRotation(glm::vec3 radians);
        Builder& setPosition(glm::vec3 position);
        Builder& setPosition(float x, float y, float z);
        Builder& setScale(glm::vec3 scales);
        Builder& setScale(float x, float y, float z);
        Builder& setColor(glm::vec3 color);
        Builder& setColor(float r, float g, float b);
        Builder & setMovement(std::shared_ptr<MovementCalculator> mc);
        Object build();
    };

};


