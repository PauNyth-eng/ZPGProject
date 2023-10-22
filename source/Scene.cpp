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
}

void Scene::Init()
{
    camera.apply();
}

void Scene::Update(double timeDelta)
{
    camera.update(timeDelta);
    for (auto& obj : objects)
    {
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
}


Scene* Scene::Builder::build()
{
    auto* scene = new Scene{
            std::move(objects),
            cameraPos
    };

    scene->camera.registerObserver(ShaderManager::constant());
    scene->camera.registerObserver(ShaderManager::lambert());
    scene->camera.registerObserver(ShaderManager::phong());
    scene->camera.registerObserver(ShaderManager::blinn());

    Mouse::instance().registerObserver(scene->camera);

    reset();
    return scene;
}
