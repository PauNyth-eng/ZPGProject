//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include <functional>
#include <optional>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Observer.h"
#include "Shader.h"
#include "Object.h"
#include "Camera.h"
#include "Scene.h"
#include "ShaderManager.h"
class Engine :
        public Observer
{
private:
    Mouse & mouse = Mouse::instance();
    Scene* scenePtr = nullptr;
    Scene& scene();
    static Engine* engine;
    size_t selected = noneSelected;

    int bufferWidth = 0;
    int bufferHeight = 0;

    GLFWwindow* window = nullptr;
    typedef decltype(std::chrono::high_resolution_clock::now()) TimePoint;
    typedef std::chrono::duration<double, std::ratio<1>> Second;

    TimePoint lastTime = std::chrono::high_resolution_clock::now();
    static Engine& instance();
    void deselect();
    void selectObject(int mouseX, int mouseY);
    void emplaceObject(const int mouseX, const int mouseY);
    void fillPicker(const int mouseX, const int mouseY);
    void deselectObject(int mouseX, int mouseY);
    void onButtonPress(const MouseData & mouseData);
    bool hasSelected();
    Object& getSelected();


    void initCallbacks();

    void printInfo();

    void initGLFW();
    void initWindow();
    void initGLFWContext();
    void initGLEW();

    void initViewport();
    void initGL();
    void initApplication();
    void initScene();
    void updatePlayer(float dt);
    void update(float dt);
    static constexpr size_t noneSelected = std::numeric_limits<std::size_t>::max();
    void notify(EventType eventType, void* object) override;
    void errorCallback(int error, const char* description);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void windowSizeCallback(GLFWwindow* window, int width, int height);
public:
    Engine();

    void run();
    ~Engine();
};
