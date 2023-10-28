//
// Created by Pavel Janu(Skola) on 10/20/2023.
//

#pragma once
enum class EventType {
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    CameraMoved,
    LightChanged
};
enum class LightType
{
    Directional = 0,
    Point,
    Spot,
    Ambient
};

enum class Growth {
    none = 0,
    grow = 1,
    shrink = -1
};

enum class Rotation {
    none = 0,
    left = 1,
    right = -1
};

enum class Direction {
    none = 0,
    up = 1,
    down = -1,
    left = -1,
    right = 1
};
