//
// Created by Pavel Janu(Skola) on 10/28/2023.
//
#pragma once


#include <vector>
#include <functional>

#include <glm/vec3.hpp>
#include "Enums.h"
#include "Observer.h"




class Light : public Observable
{
protected:
    mutable glm::vec3 color { 1.f };

public:
    Light() = default;
    explicit Light(glm::vec3 color);

    float boundColor(float color) const;
    glm::vec3 boundColor(glm::vec3 color) const;
    std::size_t index = 0;

    virtual LightType type() const;

    virtual void AddColor(glm::vec3 color);
    virtual void SetColor(glm::vec3 color);
    virtual glm::vec3 GetColor() const;

    virtual void apply() const;
};

typedef Light AmbientLight;


class DirectionalLight : public Light
{
    mutable glm::vec3 direction { 0.f };
public:

    DirectionalLight() = default;
    DirectionalLight(glm::vec3 color, glm::vec3 direction);

    LightType type() const override;

    void SetDirection(glm::vec3 direction);
    glm::vec3 GetDirection() const;

};

class PositionedLight : public Light
{
    mutable glm::vec3 position { 0.f };
public:
    PositionedLight() = default;
    PositionedLight(glm::vec3 color, glm::vec3 position);

    LightType type() const override;

    void SetPosition(glm::vec3 position) const;
    glm::vec3 GetPosition() const;

    void move(glm::vec3 offset) const;

};


class SpotLight : public PositionedLight {
    mutable glm::vec3 direction{0.f};
    mutable float cutOff = 0.f;
public:
    SpotLight() = default;
    SpotLight(glm::vec3 color, glm::vec3 position, glm::vec3 direction, float cutOff);

    LightType type() const override;

    void SetDirection(glm::vec3 direction);
    glm::vec3 GetDirection() const;

    void SetCutOff(float cutOff);
    float GetCutOff() const;


};

