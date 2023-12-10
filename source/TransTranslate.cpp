//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#include "TransTranslate.h"

glm::mat4 TransTranslate::Calculate(glm::mat4 t)
{
    return glm::translate(glm::mat4{1.f}, move);
}

TransTranslate::TransTranslate(glm::vec3 move) : move(move)
{
}