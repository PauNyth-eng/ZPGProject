//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#include "Scene.h"

Scene::Scene(std::vector<Object> objects, glm::vec3 cameraPos) : objects(std::move(objects))
{
    Init();
    camera.setPosition(cameraPos);
}


void Scene::setShaderCount() const
{
    ShaderManager::lambert().passUniformLocation("lightCount", lights.size());
    ShaderManager::blinn().passUniformLocation("lightCount", lights.size());
    ShaderManager::phong().passUniformLocation("lightCount", lights.size());
}

void Scene::Init()
{   ambient.apply();
    applyLights();
    camera.apply();
}

void Scene::Update(double timeDelta)
{

    camera.update(timeDelta);
    skybox->draw();
    for (auto& obj : objects)
    {
        obj.update(timeDelta);
        obj.draw();
    }
}

void Scene::AddAll(const std::vector<Object>& objects)
{
    for (const Object& obj : objects) {
        this->objects.emplace_back(obj);
    }
}

void Scene::AddObject(const Object& object)
{
    this->objects.emplace_back(object);
}

void Scene::removeObject(size_t id)
{
    auto iter = std::find_if(
            objects.begin(), objects.end(),
            [id](const Object& obj) -> bool { return obj.objectId() == id; }
    );
    if (iter != objects.end()) {
        objects.erase(iter);
    }
}

size_t Scene::indexOf(unsigned int objectId)
{
    for (size_t i = 0; i < objects.size(); ++i) {
        if (objects[i].objectId() == objectId) {
            return i;
        }
    }
    return std::numeric_limits<size_t>::max();
}

Scene::Scene(std::vector<Object> objects, AmbientLight ambientLight, std::vector<std::shared_ptr<Light>> lights,
             glm::vec3 cameraPos, std::shared_ptr<Skybox> skybox) : objects(std::move(objects)), ambient(std::move(ambientLight)), lights(std::move(lights)), skybox(std::move(skybox)){
    Init();
    camera.setPosition(cameraPos);
}

size_t Scene::lightCount() const {
    return lights.size();
}

const Light &Scene::light(size_t index) const {
    return *lights[index];
}
std::shared_ptr<Light> Scene::lightPtr(const size_t index) const {
    return lights[index];
}

void Scene::removeLight(std::size_t lightIndex) {
    lights.erase(lights.begin() + lightIndex);
    for (std::size_t i = lightIndex; i < lights.size(); ++i) {
        lights[i]->index = i;
    }
    applyLights();
}

void Scene::applyLights() const {
    setShaderCount();
    for (const auto & light : lights) {
        light->apply();
    }
}

static void initAndEmplace(std::shared_ptr<Light> & light, std::vector<std::shared_ptr<Light>> & vec) {

    light->registerObserver(ShaderManager::constant());
    light->registerObserver(ShaderManager::lambert());
    light->registerObserver(ShaderManager::phong());
    light->registerObserver(ShaderManager::blinn());
    light->index = vec.size();
    vec.emplace_back(light);

}

static std::shared_ptr<Light> createLight(glm::vec3 color, glm::vec3 data, LightType type) {
    if (type == LightType::Point) {
        return std::make_shared<PositionedLight>(color, data);
    } else if (type == LightType::Directional) {
        return std::make_shared<DirectionalLight>(color, data);
    }
    throw std::runtime_error("Unsupported light type");
}

static void emplaceLight(glm::vec3 color, glm::vec3 pos, glm::vec3 dir, float cutoff,
                         std::vector<std::shared_ptr<Light>> & vec) {

    std::shared_ptr<Light> light = std::make_shared<SpotLight>(color, pos, dir, cutoff);
    initAndEmplace(light, vec);
}

static void emplaceLight(const glm::vec3 & color, const glm::vec3 & pos, const LightType type,
                         std::vector<std::shared_ptr<Light>> & vec) {

    std::shared_ptr<Light> light = createLight(color, pos, type);
    initAndEmplace(light, vec);
}

void Scene::emplaceLight(glm::vec3 color, glm::vec3 position, LightType type) {
    ::emplaceLight(color, position, type, lights);
    applyLights();
}

void Scene::emplaceLight(glm::vec3 color, glm::vec3 pos, glm::vec3 dir, float cutoff) {
    ::emplaceLight(color, pos, dir, cutoff, lights);
    applyLights();
}


Scene::Builder::Builder()
{
    reset();
}

Scene::Builder& Scene::Builder::setCameraPosition(glm::vec3 position)
{
    cameraPos = position;
    return *this;
}

Scene::Builder& Scene::Builder::setCameraPosition(float x, float y, float z)
{
    cameraPos = { x, y ,z };
    return *this;
}

Scene::Builder& Scene::Builder::addAll(const std::vector<Object>& objects)
{
    for (const Object& obj : objects) {
        this->objects.emplace_back(obj);
    }
    return *this;
}

Scene::Builder& Scene::Builder::addObject(const Object& object)
{
    objects.emplace_back(object);
    return *this;
}

void Scene::Builder::reset()
{
    objects = { };
    cameraPos = { 0.f, 0.f, 0.f };
    lights = { };
    emplaceAmbientLight({ 0.1f, 0.1f, 0.1f });
}


static std::vector<std::string> cubemapTextures {
        "resources/textures/space/right.png",
        "resources/textures/space/left.png",
        "resources/textures/space/top.png",
        "resources/textures/space/bottom.png",
        "resources/textures/space/front.png",
        "resources/textures/space/back.png"
};

static std::shared_ptr<Skybox> initSkybox() {
    return std::make_shared<Skybox>(
            TextureManager::cubeMap("skybox", cubemapTextures)
    );
}

Scene* Scene::Builder::build()
{
    auto* scene = new Scene{
            std::move(objects),
            ambient,
            lights,
            cameraPos,
            initSkybox()
    };

    scene->camera.registerObserver(ShaderManager::constant());
    scene->camera.registerObserver(ShaderManager::lambert());
    scene->camera.registerObserver(ShaderManager::phong());
    scene->camera.registerObserver(ShaderManager::blinn());
    scene->camera.registerObserver(ShaderManager::skybox());

    Mouse::instance().registerObserver(scene->camera);

    reset();
    return scene;
}

Scene::Builder &Scene::Builder::emplaceLight(glm::vec3 color, glm::vec3 data, LightType type) {
    ::emplaceLight(color, data, type, lights);
    return *this;
}

Scene::Builder &Scene::Builder::emplaceLight(glm::vec3 color, glm::vec3 pos, glm::vec3 dir, float cutoff) {
    ::emplaceLight(color, pos, dir, cutoff, lights);
    return *this;
}

Scene::Builder &Scene::Builder::emplaceAmbientLight(glm::vec3 color) {
    ambient = AmbientLight { color };
    ambient.registerObserver(ShaderManager::constant());
    ambient.registerObserver(ShaderManager::lambert());
    ambient.registerObserver(ShaderManager::phong());
    ambient.registerObserver(ShaderManager::blinn());
    return *this;
}
