//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#include "TransComposite.h"

glm::mat4 TransComposite::Calculate()
{
    glm::mat4 temp{ 1.f };

    for (auto& trans : tranformations)
    {
        temp *= trans->Calculate();
    }

    return temp;

}

void TransComposite::AddTransformation(TransComponent* component)
{
    tranformations.push_back(component);
}
