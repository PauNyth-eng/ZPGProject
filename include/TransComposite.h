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
    glm::mat4 Calculate() override;
    void AddTransformation(TransComponent* component);
private:
    std::vector<TransComponent*> tranformations;
};
