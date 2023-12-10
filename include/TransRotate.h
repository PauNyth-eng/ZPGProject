//
// Created by Pavel Janu(Skola) on 10/20/2023.
//

#pragma once
#include "TransComponent.h"
class TransRotate :
        public TransComponent
{
public:
    TransRotate(float radians, glm::vec3 way);
    glm::mat4 Calculate(glm::mat4 t) override;
    float radians;
    glm::vec3 way;

};
