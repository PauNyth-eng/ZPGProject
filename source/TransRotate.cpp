//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#include "TransRotate.h"

TransRotate::TransRotate(float radians, glm::vec3 direction) : radians(radians), direction(direction)
{
}

glm::mat4 TransRotate::Calculate()
{
    glm::mat4 temp =  glm::rotate(glm::mat4{ 1.f }, radians, direction);
    return temp;
}
