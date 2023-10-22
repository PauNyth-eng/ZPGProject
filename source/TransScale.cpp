//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#include "TransScale.h"

TransScale::TransScale(glm::vec3 scales) : scales(scales)
{
}

glm::mat4 TransScale::Calculate()
{
    glm::mat4 temp = glm::scale(glm::mat4{ 1.f }, scales);
    return temp;
}
