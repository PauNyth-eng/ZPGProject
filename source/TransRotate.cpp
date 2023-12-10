//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#include "TransRotate.h"


TransRotate::TransRotate(float radians, glm::vec3 direction, glm::vec3 rotationCenter, float radius)
        : radians(radians), direction(direction), rotationCenter(rotationCenter), radius(radius)
{
}

glm::mat4 TransRotate::Calculate()
{
    glm::mat4 translation1 = glm::translate(glm::mat4(1.0f), -rotationCenter);

    glm::mat4 radiusTranslation = glm::translate(glm::mat4(1.0f), radius * direction);

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), radians, direction);

    glm::mat4 translation2 = glm::translate(glm::mat4(1.0f), rotationCenter);

    glm::mat4 result = translation2 * rotation * radiusTranslation * translation1;

    return result;
}