//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#include "Camera.h"
#include <cmath>
void Camera::updateCameraMatrix()
{
    camera = glm::lookAt(eye, eye + target, up);
}

void Camera::calcTarget()
{
    target = glm::vec3{ std::cos(fi), std::sin(psi), std::sin(fi) };
}

void Camera::capAngles()
{
    if (fi < 0) {
        fi += 2*M_PI;
    }
    fi = std::fmod(fi, 2*M_PI);

    psi = std::max(psi, (float)-(M_PI_2*0.95));
    psi = std::min(psi, (float)(M_PI_2*0.95));
}

void Camera::updateAngle(float dt)
{
    const float dFi = hRotate * dt * rotationSpeed;
    const float dPsi = vRotate * dt * rotationSpeed;

    fi += dFi;
    psi += dPsi;

    capAngles();

    changeMade = changeMade or dFi or dPsi;
}

Camera::Camera()
{
    updateCameraMatrix();
}

void Camera::setPosition(glm::vec3 pos)
{
    eye = pos;
    calcTarget();
    updateCameraMatrix();
    apply();
}

void Camera::move(glm::vec3 delta)
{
    eye += delta;
    apply();
}

void Camera::moveSideways(Direction dir)
{
    movement.y = (float)dir;
}

void Camera::moveForward(Direction dir)
{
    movement.x = (float)dir;
}

void Camera::moveUpDown(Direction dir)
{
    movement.z = (float)dir;
}


void Camera::rotateHor(Direction dir)
{
    hRotate = (float)dir;
}

void Camera::rotateVer(Direction dir)
{
    vRotate = (float)dir;
}

static glm::vec3 adjustMovementSpeed(const glm::vec3 vector, const float max) {
    const float fx = (vector.y != 0) ? vector.x / 2 : vector.x;
    const float fy = (vector.x != 0) ? vector.y / 2 : vector.y;
    const float fz = (vector.z != 0) ? vector.z / 2 : vector.z;
    return { fx, fy, fz};
}

glm::vec3 Camera::movementVector(float dt) const {
    const glm::vec3 adjusted = adjustMovementSpeed(movement, moveSpeed);
    const float sideways = adjusted.y * dt * moveSpeed;
    const float forward = adjusted.x * dt * moveSpeed;
    const float upDown = adjusted.z * dt * moveSpeed;

    const float dx = std::cos(fi) * forward + std::cos(fi + M_PI_2) * sideways;
    const float dz = std::sin(fi) * forward + std::sin(fi + M_PI_2) * sideways;

    return { dx, dz, upDown };
}
void Camera::update(float dt)
{
    updateAngle(dt);
    calcTarget();
    updateCameraMatrix();

    if (changeMade) {
        apply();
        changeMade = false;
    }
}

void Camera::apply()
{
    notifyObservers(EventType::CameraMoved, this);
}

void Camera::notify(EventType eventType, void* object)
{
    if (eventType == EventType::MouseMoved) {
        onMouseMove(((Mouse*)object)->data());
    }
}

void Camera::onMouseMove(const MouseData & md) {

    if (not md.mbPressed()) {
        return;
    }

    const float rad_x = md.dx / 180.f * M_PI;
    const float rad_y = md.dy / 180.f * M_PI;

    const float dFi = rad_x * dragSpeed;
    const float dPsi = -1.f * rad_y * dragSpeed;

    fi += dFi;
    psi += dPsi;

    capAngles();

    changeMade = dFi or dPsi;

}

glm::mat4 Camera::view() const
{
    return camera;
}

glm::mat4 Camera::projection() const
{
    return projectMat;
}

glm::vec3 Camera::position() const
{
    return eye;
}
