//
// Created by Pavel Janu(Skola) on 10/20/2023.
//

#pragma once

#include <vector>
#include "Camera.h"
#include "Object.h"
#include "ShaderManager.h"
#include "Light.h"
class Scene
{
private:
    Scene(std::vector<Object> objects, AmbientLight ambientLight,
          std::vector<std::shared_ptr<Light>> light, glm::vec3 cameraPos);
    void applyLights() const;
    std::vector<std::shared_ptr<Light>> lights;
public:
    Scene(std::vector<Object> objects, glm::vec3 cameraPos);
    void setShaderCount() const;

    Scene() = delete;

    std::vector<Object> objects;
    const AmbientLight ambient;
    Camera camera;


    size_t lightCount() const;

    const Light & light(size_t index) const;
    std::shared_ptr<Light> lightPtr(size_t index) const;

    void removeLight(std::size_t lightIndex);
    void emplaceLight(glm::vec3 color, glm::vec3 position, LightType type);
    void emplaceLight(glm::vec3 color, glm::vec3 pos, glm::vec3 dir, float cutoff);
    void Init();

    void Update(double timeDelta);

    void AddAll(const std::vector<Object>& objects);

    void AddObject(const Object& object);

    void removeObject(size_t id);


    size_t indexOf(unsigned int objectId);

    class Builder{

        std::vector<Object> objects;
        glm::vec3 cameraPos { 0.f };
        std::vector<std::shared_ptr<Light>> lights;
        AmbientLight ambient;
        void reset();

    public:

        Builder();
        Builder & emplaceLight(glm::vec3 color, glm::vec3 data, LightType type);
        Builder & emplaceLight(glm::vec3 color, glm::vec3 pos, glm::vec3 dir, float cutoff);
        Builder & emplaceAmbientLight(glm::vec3 color);
        Builder& setCameraPosition(glm::vec3 position);
        Builder& setCameraPosition(float x, float y, float z);

        Builder& addAll(const std::vector<Object>& objects);
        Builder& addObject(const Object& object);


        Scene* build();
    };
};


