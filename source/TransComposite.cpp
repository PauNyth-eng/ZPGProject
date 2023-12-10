//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#include "TransComposite.h"

glm::mat4 TransComposite::Calculate(glm::mat4 t)
{

    for (auto& trans : tranformations)
    {
        t *= trans->Calculate(t);
    }

    return t;

}

void TransComposite::AddTransformation(std::shared_ptr<TransComponent> component)
{
    tranformations.push_back(component);
}
