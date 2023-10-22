//
// Created by Pavel Janu(Skola) on 10/20/2023.
//

#pragma once
#include <memory>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TransComponent
{
public:
    virtual glm::mat4 Calculate() = 0;
};

