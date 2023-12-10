//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#include "TransRotate.h"


TransRotate::TransRotate(float radians, glm::vec3 way)
        : radians(radians), way(way)
{
}

glm::mat4 TransRotate::Calculate(glm::mat4 t)
{
    return glm::rotate(glm::mat4{1.f}, radians, way);
}