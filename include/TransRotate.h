//
// Created by Pavel Janu(Skola) on 10/20/2023.
//

#pragma once
#include "TransComponent.h"
class TransRotate :
        public TransComponent
{
public:
    TransRotate(float radians, glm::vec3 direction, glm::vec3 rotationCenter, float radius = 0);
    glm::mat4 Calculate() override;
private:
    float radians;
    glm::vec3 direction;
    glm::vec3 rotationCenter;
    float radius;
};
