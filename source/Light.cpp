//
// Created by Pavel Janu(Skola) on 10/28/2023.
//

#include "Light.h"


float Light::boundColor(const float color) const
{
    return std::max(std::min(1.f, color), 0.f);
}

glm::vec3 Light::boundColor(const glm::vec3 color) const
{
    return {boundColor(color.x), boundColor(color.y), boundColor(color.y)};
}

Light::Light(glm::vec3 color) : color(boundColor(color))
{

}

void Light::AddColor(glm::vec3 color)
{
    SetColor(this->color + color);
}

void Light::SetColor(glm::vec3 color)
{
    this->color = boundColor(color);
    apply();
}

glm::vec3 Light::GetColor() const
{
    return color;
}

void Light::apply() const
{
    notifyObservers(EventType::LightChanged, (void *)this);
}

LightType Light::type() const
{
    return LightType::Ambient;
}



DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 direction) : Light(color), direction(direction)
{

}

LightType DirectionalLight::type() const
{
    return LightType::Directional;
}

void DirectionalLight::SetDirection(glm::vec3 direction)
{
    this->direction = direction;
    apply();
}

glm::vec3 DirectionalLight::GetDirection() const
{
    return direction;
}

PositionedLight::PositionedLight(glm::vec3 color, glm::vec3 position) : Light(color), position(position)
{

}

LightType PositionedLight::type() const
{
    return LightType::Point;
}

void PositionedLight::SetPosition(const glm::vec3 position) const
{
    this->position = position;
    apply();
}

glm::vec3 PositionedLight::GetPosition() const
{
    return position;
}

void PositionedLight::move(const glm::vec3 offset) const
{
    SetPosition(position + offset);
}

SpotLight::SpotLight(glm::vec3 color, glm::vec3 position, glm::vec3 direction, float cutOff) : PositionedLight(color, position), direction(direction), cutOff(cutOff)
{

}

LightType SpotLight::type() const
{
    return LightType::Spot;
}

void SpotLight::SetDirection(glm::vec3 direction)
{
    this->direction = direction;
    apply();
}

glm::vec3 SpotLight::GetDirection() const
{
    return direction;
}

void SpotLight::SetCutOff(float cutOff)
{
    this->cutOff = cutOff;
    apply();
}

float SpotLight::GetCutOff() const
{
    return cutOff;
}

