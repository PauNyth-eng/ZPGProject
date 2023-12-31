//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#pragma once
#include "TransComponent.h"
class TransScale :
        public TransComponent
{
public:
    explicit TransScale(glm::vec3 scales);
    glm::mat4 Calculate(glm::mat4 t) override;
    glm::vec3 scales;
};
