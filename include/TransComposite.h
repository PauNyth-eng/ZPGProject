//
// Created by Pavel Janu(Skola) on 10/20/2023.
//

#pragma once
#include "TransComponent.h"
#include <vector>
class TransComposite :
        public TransComponent
{
public:
    glm::mat4 Calculate(glm::mat4 t) override;
    void AddTransformation(std::shared_ptr<TransComponent> component);
    std::vector<std::shared_ptr<TransComponent>> tranformations;
};
