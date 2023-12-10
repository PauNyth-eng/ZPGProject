//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#include "Engine.h"

Engine * Engine::engine = nullptr;
void Engine::errorCallback(int error, const char * description) {
    (void)error;
    std::cerr << description << std::endl;
}


void Engine::windowSizeCallback(GLFWwindow *window, int width, int height)
{
    (void)window;
    bufferWidth = width;
    bufferHeight = height;
    scene().camera.updateProjectionMatrix(bufferWidth,bufferHeight);
    glViewport(0, 0, bufferWidth, bufferHeight);
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
        case GLFW_KEY_U:
            break;
        case GLFW_KEY_J:
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

    auto windowSize = [](GLFWwindow* window, int width, int height) {
        instance().windowSizeCallback(window, width, height);
    };
    glfwSetWindowSizeCallback(window, windowSize);

    auto cursor = [](GLFWwindow * win, double x, double y) {
        instance().mouse.MouseMove((int)x, (int)y);
    };
    glfwSetCursorPosCallback(window, cursor);

    auto mouseButton = [](GLFWwindow * win, int button, int action, int mode) {
        Button btn = Button::Other;
        if (button == GLFW_MOUSE_BUTTON_1) {
            btn = Button::LEFT;
        } else if (button == GLFW_MOUSE_BUTTON_2) {
            btn = Button::RIGHT;
        } else if (button == GLFW_MOUSE_BUTTON_3) {
            btn = Button::MIDDLE;
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
    Mouse::instance().registerObserver(*engine);
}



void Engine::run()
{
    lastTime = std::chrono::high_resolution_clock::now();

    glClearColor(0.f, 0.f, 0.4f, 0.f);
    update(0.0);
    scene().Init();
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
    if (hasSelected()) {
        getSelected().setColor(Object::defaultColor);
        selected = noneSelected;
    }
}

void Engine::selectObject(int mouseX, int mouseY)
{
    GLuint objectId;

    const GLint x = mouseX;
    const GLint y = bufferHeight - mouseY;

    glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &objectId);

    std::cout << "Selected object: " << objectId << std::endl;

    deselect();
    if (objectId) {

        selected = scene().indexOf(objectId);
        // getSelected().setColor(Object::secondaryColor);
    }
}

void Engine::deselectObject(int mouseX, int mouseY)
{
    deselect();
}

bool Engine::hasSelected()
{
    return selected != noneSelected and selected < scene().objects.size();
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
    glViewport(0, 0, bufferWidth, bufferHeight);
}

void Engine::initGL()
{
    initGLFW();
    initWindow();
    initGLFWContext();
    initGLEW();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
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

    scenePtr = sceneBuilder
            .emplaceAmbientLight(glm::vec3{0.0001f})
            //.emplaceLight(glm::vec3 { 1.f, 0.f, 0.f }, glm::vec3 { -10.f, 3.f, -5.f }, glm::vec3 { 0.f, -1.f, 0.f }, 30.f)
            //.emplaceLight(glm::vec3 { 1.f, 0.f, 0.f }, glm::vec3 { -10.f, 3.f, -5.f }, glm::vec3 { 0.f, 1.f, 0.f }, 30.f)
            //.emplaceLight(glm::vec3 { 1.f, 0.f, 0.f }, glm::vec3 { -10.f, 3.f, -5.f }, glm::vec3 { 1.f, 0.f, 0.f }, 30.f)
            //.emplaceLight(glm::vec3 { 1.f, 0.f, 0.f }, glm::vec3 { -10.f, 3.f, -5.f }, glm::vec3 { -1.f, 0.f, 0.f }, 30.f)
            .emplaceLight(glm::vec3 { 0.f, -1.f, 0.f }, glm::vec3 { 0.f, -1.f, 0.f }, LightType::Directional)
            .addObject(
                    objBuilder
                            .emplaceObject(ModelLoader::get("Plane/untitled"), ShaderManager::lambert(), TextureManager::getOrEmplace("grass", "resources/textures/grass.png"))
                            .setPosition(0.f, 10.f, 0.f).setScale(0.005f, 0.005f, 0.005f)
                            .setMovement(
                                    std::make_shared<MovementCalculator>(
                                            std::make_shared<BezierCurve>(
                                                    glm::vec3 { 80.f, 20.f, 50.f },
                                                    glm::vec3 { 20.f, 20.f, -50.f },
                                                    glm::vec3 { -30.f, 20.f, 20.f },
                                                    glm::vec3 { -80.f, 20.f, 10.f }
                                            ),
                                            glm::vec3 { 0.0, 0.0, 0.0 },
                                            10.0
                                    )
                            )
                            .build()
            )
            .addObject(
                        objBuilder
                        .emplaceObject(ModelLoader::get("Terrain/teren"), ShaderManager::lambert(), TextureManager::getOrEmplace("grass", "resources/textures/grass.png"))
                        .setPosition(0.f, 0.f, 0.f).setScale(glm::vec3{2.f})
                        .build()
                    )
            .setCameraPosition(0.f, 6.f, 0.f)
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
    if (mouseData.lbPressed()) {
        selectObject(mouseData.x, mouseData.y);
    } else if (mouseData.rbPressed()) {
        emplaceObject(mouseData.x, mouseData.y);
    }
}

void Engine::notify(EventType eventType, void* object)
{
    if (eventType == EventType::MouseButtonPressed) {
        onButtonPress(((Mouse*)object)->data());
    }
}

void Engine::emplaceObject(const int mouseX, const int mouseY) {
    GLfloat depth;

    const GLint x = mouseX;
    const GLint y = bufferHeight - mouseY;

    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

    glm::vec3 screenX = glm::vec3 { x, y, depth };
    glm::vec4 viewport { 0, 0, bufferWidth, bufferHeight };
    auto pos = glm::unProject(screenX, scene().camera.view(), scene().camera.projection(), viewport);

    scene().objects.emplace_back(
            Object::Builder()
                    .emplaceObject(ModelLoader::get("Plane/untitled"), ShaderManager::phong(),
                                   TextureManager::get("grass"))
                    .setPosition(pos.x, pos.y, pos.z).setScale(0.001f, 0.001f, 0.001f)
                    .build()
    );
}
