//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#pragma once
#include <functional>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Observer.h"
#include "Enums.h"
#include "Mouse.h"
class Camera :
        public Observer, public Observable
{
private:
    glm::mat4 projectMat = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 200.0f);

    glm::vec3 eye{ 0.f, 1.f, 3.f };
    glm::vec3 target{ 0., 5., 0. };
    glm::vec3 up{ 0.f, 6.f, 0.f };

    float fi = 1.5f * M_PI;
    float psi = 0.f;

    glm::vec3 movement{ 0.0, 0.0, 0.0 };
    float hRotate = 0;
    float vRotate = 0;
    glm::mat4 camera;
    bool changeMade = false;
    static constexpr float moveSpeed = 7.f;
    static constexpr float rotationSpeed = 1.f;
    static constexpr float dragSpeed = 0.3f;

    void updateCameraMatrix();
    void calcTarget();

    void capAngles();

    void updateAngle(float dt);

public:
    Camera();

    void setPosition(glm::vec3 pos);
    void move(glm::vec3 delta);
    glm::vec3 getTarget() const;
    void moveSideways(Direction dir);
    void moveForward(Direction dir);
    void moveUpDown(Direction dir);
    void rotateHor(Direction dir);
    void rotateVer(Direction dir);

    glm::vec3 movementVector(float dt) const;

    void update(float dt);
    void onMouseMove(const MouseData & md);
    void apply();

    void notify(EventType eventType, void* object) override;

    glm::mat4 view() const;
    glm::mat4 projection() const;
    glm::vec3 position() const;
};


#