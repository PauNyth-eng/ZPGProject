//
// Created by Pavel Janu(Skola) on 12/10/2023.
//

#include "Movement.h"

#include <iostream>

/* -------------------- Line -------------------- */

static glm::vec3 calcDir(const glm::vec3 begin, const glm::vec3 end) {

    glm::vec3 dir = glm::vec3 { end.x, 0.f, end.z } - glm::vec3 { begin.x, 0.f, begin.z };
    dir = glm::normalize(dir);
    std::cout << dir.x << ", " << dir.z << std::endl;

    float angle = std::acos(glm::dot(dir, glm::vec3 { 1.f, 0.f, 0.f }));
    if (dir.z < 0.f) {
        angle = 2*M_PI - angle;
    }

    std::cout << angle << std::endl;
    return { 0.f, -angle, 0.f };
}

Line::Line(glm::vec3 begin, glm::vec3 end) : begin(begin), end(end), dir(calcDir(begin, end)) { }

static glm::vec3 operator*(const glm::vec3 vec, const float f) {
    return { vec.x * f, vec.y * f, vec.z * f };
}

glm::vec3 Line::position(const float t) const {
    return begin * (1.f - t) + end * t;
}

glm::vec3 Line::direction(const float t) const {
    (void)t;
    return dir;
}

/* -------------------- Circle -------------------- */

float Circle::calcAngle(const float t) {
    constexpr float twopi = 2.f * M_PI;
    return twopi * t;
}

Circle::Circle(const glm::vec3 center, const float radius) : center(center), radius(radius), r2(radius*radius) { }

glm::vec3 Circle::position(const float t) const {
    const float angle = calcAngle(t);
    const float x = center.x + std::cos(angle) * radius;
    const float y = center.y;
    const float z = center.z + std::sin(angle) * radius;
    return { x, y, z };
}

glm::vec3 Circle::direction(const float t) const {
    const float angle = calcAngle(t);
    return { 0.f, -angle-M_PI_2, 0.f };
}

/* -------------------- Bezier Curve -------------------- */

static auto createCallback(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4) {

    return [=](const float t) -> glm::vec3 {
        const glm::vec3 point1 = p1 * std::pow(1-t, 3.f);
        const glm::vec3 point2 = p2 * 3*t * std::pow(1-t, 2.f);
        const glm::vec3 point3 = p3 * 3*t*t * (1-t);
        const glm::vec3 point4 = p4 * t*t*t;
        return point1 + point2 + point3 + point4;
    };
}

BezierCurve::BezierCurve(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4) :
        bezierCallback(createCallback(p1, p2, p3, p4)) { }

glm::vec3 BezierCurve::position(float t) const {
    return bezierCallback(t);
}

glm::vec3 BezierCurve::direction(float t) const {
    return glm::vec3();
}

/* -------------------- Calculator -------------------- */

MovementCalculator::MovementCalculator(std::shared_ptr<MovementCurve> curve, glm::vec3 rotationOffset, float duration) :
        curve(std::move(curve)), offset(rotationOffset), duration(duration) { }

float MovementCalculator::currentT() const {
    return t / duration;
}

void MovementCalculator::update(float dt) {
    t = std::fmod(t+dt, duration);
}

glm::vec3 MovementCalculator::currentPosition() const {
    return curve->position(currentT());
}

glm::vec3 MovementCalculator::currentHeading() const {
    return curve->direction(currentT()) + offset;
}
