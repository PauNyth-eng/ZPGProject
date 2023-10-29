//
// Created by Pavel Janu(Skola) on 10/20/2023.
//

#pragma once
#include "Shader.h"
class ShaderManager
{
    Shader _constant;
    Shader _lambert;
    Shader _phong;
    Shader _phong2;
    Shader _blinn;


    static ShaderManager* sm;
    static ShaderManager& instance();


    ShaderManager();
public:
    static Shader& constant();
    static Shader& lambert();
    static Shader& phong();
    static Shader& phong2();
    static Shader& blinn();
};

