//
// Created by Pavel Janu(Skola) on 10/27/2023.
//
#include "Engine.h"


int main() {
    auto engine = new Engine();
    engine->run();

    delete engine;
    return 0;
}