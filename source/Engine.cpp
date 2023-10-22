//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#include "Engine.h"

Engine * Engine::engine = nullptr;
void Engine::errorCallback(int error, const char * description) {
    (void)error;
    std::cerr << description << std::endl;
}


void Engine::windowFocusCallback(GLFWwindow *window, int focused)
{
    (void)window;
    (void)focused;

}

void Engine::windowIconifyCallback(GLFWwindow *window, int iconified)
{
    (void)window;
    (void)iconified;
}

void Engine::windowSizeCallback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

void Engine::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;
    if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (not scenePtr)
    {
    return;
    }

    switch (key)
    {
        case GLFW_KEY_W:
            scene().camera.moveForward((action == GLFW_RELEASE) ? Direction::none : Direction::up);
            break;
        case GLFW_KEY_S:
            scene().camera.moveForward((action == GLFW_RELEASE) ? Direction::none : Direction::down);
            break;
        case GLFW_KEY_A:
            scene().camera.moveSideways((action == GLFW_RELEASE) ? Direction::none : Direction::left);
            break;
        case GLFW_KEY_D:
            scene().camera.moveSideways((action == GLFW_RELEASE) ? Direction::none : Direction::right);
            break;
        case GLFW_KEY_SPACE:
            scene().camera.moveUpDown((action == GLFW_RELEASE) ? Direction::none : Direction::up);
            break;
        case GLFW_KEY_LEFT_SHIFT:
            scene().camera.moveUpDown((action == GLFW_RELEASE) ? Direction::none : Direction::down);
            break;
        case GLFW_KEY_UP:
            scene().camera.rotateVer((action == GLFW_RELEASE) ? Direction::none : Direction::up);
            break;
        case GLFW_KEY_DOWN:
            scene().camera.rotateVer((action == GLFW_RELEASE) ? Direction::none : Direction::down);
            break;
        case GLFW_KEY_LEFT:
            scene().camera.rotateHor((action == GLFW_RELEASE) ? Direction::none : Direction::left);
            break;
        case GLFW_KEY_RIGHT:
            scene().camera.rotateHor((action == GLFW_RELEASE) ? Direction::none : Direction::right);
            break;
        default:
            break;
    }
}

Engine& Engine::instance()
{
    if (not engine)
    {
        engine = new Engine();
    }

    return *engine;

}

void Engine::initCallbacks()
{
    auto windowError = [](int error, const char* description){
        instance().errorCallback(error, description);
    };
    glfwSetErrorCallback(windowError);

    auto windowKey = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        instance().keyCallback(window, key, scancode, action, mods);
    };
    glfwSetKeyCallback(window, windowKey);


    auto windowFocus = [](GLFWwindow* window, int focused) {
        instance().windowFocusCallback(window, focused);
    };
    glfwSetWindowFocusCallback(window, windowFocus);

    auto windowIconify = [](GLFWwindow* window, int iconified) {
        instance().windowIconifyCallback(window, iconified);
    };
    glfwSetWindowIconifyCallback(window, windowIconify);


    auto windowSize = [](GLFWwindow* window, int width, int height) {
        instance().windowSizeCallback(window, width, height);
    };
    glfwSetWindowSizeCallback(window, windowSize);

    auto cursor = [](GLFWwindow * win, double x, double y) {
        instance().mouse.MouseMove((int)x, (int)y);
    };
    glfwSetCursorPosCallback(window, cursor);

    auto mouseButton = [](GLFWwindow * win, int button, int action, int mode) {
        Mouse::Button btn = Mouse::Button::Other;
        if (button == GLFW_MOUSE_BUTTON_1) {
            btn = Mouse::Button::LEFT;
        } else if (button == GLFW_MOUSE_BUTTON_2) {
            btn = Mouse::Button::RIGHT;
        } else if (button == GLFW_MOUSE_BUTTON_3) {
            btn = Mouse::Button::MIDDLE;
        }
        if (action == GLFW_PRESS) {
            instance().mouse.ButtonPress(btn);
        } else if (action == GLFW_RELEASE) {
            instance().mouse.ButtonRelease(btn);
        }
    };
    glfwSetMouseButtonCallback(window, mouseButton);
}

Engine::Engine()
{
    initGL();
    initApplication();
    initScene();
    engine = this;
}



void Engine::run()
{
    lastTime = std::chrono::high_resolution_clock::now();


    update(0.0);
    scene().Init();
    glEnable(GL_DEPTH_TEST);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    while (not glfwWindowShouldClose(window)) {

        const TimePoint now = std::chrono::high_resolution_clock::now();
        const float delta = std::chrono::duration_cast<Second>(now - lastTime).count();
        update(delta);
        lastTime = now;
    }
}

Engine::~Engine()
{
    glfwDestroyWindow(window);
    glfwTerminate();
    delete scenePtr;
}

Scene& Engine::scene()
{
    if (not scenePtr) {
        throw std::runtime_error("Application: Scene is not initialized");
    }
    return *scenePtr;
}

void Engine::deselect()
{
}

void Engine::selectObject(int mouseX, int mouseY)
{
}

void Engine::emplaceObject(int mouseX, int mouseY)
{
}

bool Engine::hasSelected()
{
    return false;
}

Object& Engine::getSelected()
{
    if (not hasSelected()) {
        throw std::runtime_error("No object selected");
    }
    return scene().objects[selected];
}


void Engine::printInfo()
{
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "Vendor " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    std::cout << "Using GLFW " << major << '.' << minor << '.' << revision << std::endl;
}

void Engine::initGLFW()
{
    if (not glfwInit()) {
        const char* dsc;
        glfwGetError(&dsc);
        std::cout << dsc << std::endl;
        throw std::runtime_error("ERROR: could not start GLFW3");
    }
}

void Engine::initWindow()
{
    window = glfwCreateWindow(800, 600, "ZPG", nullptr, nullptr);
    if (not window) {
        glfwTerminate();
        throw std::runtime_error("Window could not be initialized");
    }
}

void Engine::initGLFWContext()
{
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

void Engine::initGLEW()
{
    glewExperimental = GL_TRUE;
    glewInit();
}

void Engine::initViewport()
{
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    bufferRatio = bufferWidth / (float)bufferHeight;
    glViewport(0, 0, bufferWidth, bufferHeight);
}

void Engine::initGL()
{
    initGLFW();
    initWindow();
    initGLFWContext();
    initGLEW();
    printInfo();
}

void Engine::initApplication()
{
    initCallbacks();
    initViewport();
}

void Engine::initScene()
{
    Object::Builder objBuilder;
    Scene::Builder sceneBuilder;

//    scenePtr = sceneBuilder
//            .addObject(
//                    objBuilder
//                            .emplaceObject(ModelLoader::get("sphere"), ShaderManager::phong()).setScale(.5f, .5f, .5f).setPosition(3.f, 0.f, 0.f).build()
//
//            )
//            .addObject(
//                    objBuilder
//                            .emplaceObject(ModelLoader::get("sphere"), ShaderManager::phong()).setScale(.5f, .5f, .5f).setPosition(-3.f, 0.f, 0.f).build()
//
//            )
//            .addObject(
//                    objBuilder
//                            .emplaceObject(ModelLoader::get("sphere"), ShaderManager::phong()).setScale(.5f, .5f, .5f).setPosition(0.f, 0.f, 3.f).build()
//
//            ).addObject(
//                    objBuilder
//                            .emplaceObject(ModelLoader::get("sphere"), ShaderManager::phong()).setScale(.5f, .5f, .5f).setPosition(0.f, 0.f, -3.f).build()
//
//            )
//            .setCameraPosition(glm::vec3(0.f, 0.f, 0.f))
//            .build();

//    scenePtr = sceneBuilder
//            .addObject(
//                    objBuilder
//                            .emplaceObject(ModelLoader::get("sphere"), ShaderManager::phong()).setScale(.5f, .5f, .5f).setPosition(3.f, 0.f, 0.f).build()
//                    )
//                    .setCameraPosition(6.f, 0.f, 0.f)
//                    .build();

        scenePtr = sceneBuilder
            .addObject(
                    objBuilder
                            .emplaceObject(ModelLoader::get("sphere"), ShaderManager::constant()).setScale(.5f, .5f, .5f).setPosition(3.f, 0.f, 0.f).build()

            )
            .addObject(
                    objBuilder
                            .emplaceObject(ModelLoader::get("monkey"), ShaderManager::lambert()).setScale(.5f, .5f, .5f).setPosition(-3.f, 0.f, 0.f).build()

            )
            .addObject(
                    objBuilder
                            .emplaceObject(ModelLoader::get("tree"), ShaderManager::phong()).setScale(.5f, .5f, .5f).setPosition(0.f, 0.f, 3.f).build()

            ).addObject(
                    objBuilder
                            .emplaceObject(ModelLoader::get("Asuna"), ShaderManager::blinn()).setScale(.03f, .03f, .03f).setPosition(0.f, 0.f, -3.f).build()

            )
            .setCameraPosition(glm::vec3(0.f, 0.f, 0.f))
            .build();
}

void Engine::updatePlayer(float dt)
{
    auto d2 = scene().camera.movementVector(dt);
    auto pos = scene().camera.position();
    pos.x += d2.x;
    pos.z += d2.y;
    pos.y += d2.z;
    scene().camera.setPosition(pos);
}

void Engine::update(float dt)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    updatePlayer(dt);
    scene().Update(dt);

    glfwPollEvents();
    glfwSwapBuffers(window);
}

void Engine::onButtonPress(const MouseData & mouseData) {
//    if (mouseData.lbPressed()) {
//        selectObject(mouseData.x, mouseData.y);
//    } else if (mouseData.rbPressed()) {
//        emplaceObject(mouseData.x, mouseData.y);
//    }
}

void Engine::notify(EventType eventType, void* object)
{
    if (eventType == EventType::MouseButtonPressed) {
        onButtonPress(((Mouse*)object)->data());
    }
}
