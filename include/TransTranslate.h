//
// Created by Pavel Janu(Skola) on 10/20/2023.
//

#pragma once
#include "TransComponent.h"
class TransTranslate :
        public TransComponent
{
public:
    TransTranslate(glm::vec3 move);
    glm::mat4 Calculate() override;
private:
    glm::vec3 move;
};

