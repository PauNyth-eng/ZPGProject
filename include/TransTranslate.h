//
// Created by Pavel Janu(Skola) on 10/20/2023.
//

#pragma once
#include "TransComponent.h"
class TransTranslate :
        public TransComponent
{
public:
    explicit TransTranslate(glm::vec3 move);
    glm::mat4 Calculate(glm::mat4 t) override;
    glm::vec3 move;

};

