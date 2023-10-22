//
// Created by Pavel Janu(Skola) on 10/21/2023.
//
#include "Mouse.h"
#include <algorithm>

Mouse * Mouse::mouse = nullptr;

bool MouseData::lbPressed() const
{
    return button & static_cast<uint8_t>(Mouse::Button::LEFT);
}

bool MouseData::rbPressed() const
{
    return button & static_cast<uint8_t>(Mouse::Button::RIGHT);
}

bool MouseData::mbPressed() const
{
    return button & static_cast<uint8_t>(Mouse::Button::MIDDLE);
}

MouseData::MouseData() : x(0), y(0), dx(0), dy(0), button(0)
{
}

MouseData::MouseData(int x, int y, int dx, int dy, uint8_t button) : x(x), y(y), dx(dx), dy(dy), button(button)
{
}

void Mouse::ButtonPress(Mouse::Button button) {
    pressed |= static_cast<uint8_t>(button);

    mouseData = MouseData(x, y, 0, 0, pressed);

    notifyObservers(EventType::MouseButtonPressed, this);
}


void Mouse::ButtonRelease(const Button button) {
    pressed &= ~(uint8_t)button;

    mouseData = MouseData(x, y, 0, 0, pressed);

    notifyObservers(EventType::MouseButtonReleased, this);
}

Mouse & Mouse::instance() {

    if (not mouse) {
        mouse = new Mouse();
    }
    return *mouse;
}

void Mouse::MouseMove(const int nx, const int ny) {

    const int dx = nx - x;
    const int dy = ny - y;

    x = nx;
    y = ny;

    mouseData = MouseData(x, y, dx, dy, pressed);

    notifyObservers(EventType::MouseMoved, this);
}

const MouseData & Mouse::data() const {
    return mouseData;
}


Mouse::Mouse() = default;