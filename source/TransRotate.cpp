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
    // Step 1: Translate the object to make the rotationCenter the origin
    glm::mat4 translation1 = glm::translate(glm::mat4(1.0f), -rotationCenter);

    // Step 2: Translate the object from the origin to the desired radius along the specified direction
    glm::mat4 radiusTranslation = glm::translate(glm::mat4(1.0f), radius * direction);

    // Step 3: Apply the rotation transformation
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), radians, direction);

    // Step 4: Translate the object back to its original position
    glm::mat4 translation2 = glm::translate(glm::mat4(1.0f), rotationCenter);

    // Combine the transformations
    glm::mat4 result = translation2 * rotation * radiusTranslation * translation1;

    return result;
}