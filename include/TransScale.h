//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#pragma once
#include "TransComponent.h"
class TransScale :
        public TransComponent
{
public:
    TransScale(glm::vec3 scales);
    glm::mat4 Calculate() override;
private:
    glm::vec3 scales;
};
