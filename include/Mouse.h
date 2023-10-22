//
// Created by Pavel Janu(Skola) on 10/21/2023.
//
#pragma once


#include <functional>
#include <vector>
#include <memory>
#include <cstdint>


#include "Observer.h"

struct MouseData
{
    int x;
    int y;
    int dx;
    int dy;
    uint8_t button;

    [[nodiscard]] bool lbPressed() const;
    [[nodiscard]] bool rbPressed() const;
    [[nodiscard]] bool mbPressed() const;

    MouseData();
    MouseData(int x, int y, int dx, int dy, uint8_t button);

};


class Mouse : public Observable{
    static Mouse *mouse;

    int x = 0;
    int y = 0;
    uint8_t pressed = 0;

    MouseData mouseData;

    Mouse();

public:

    enum class Button {
        LEFT = 1,
        RIGHT = 2,
        MIDDLE = 4,
        Other = 8
    };

    void ButtonPress(Button button);

    void ButtonRelease(Button button);

    void MouseMove(int nx, int ny);

    const MouseData &data() const;

    static Mouse &instance();
};