//
// Created by Pavel Janu(Skola) on 10/20/2023.
//

#pragma once

#include <vector>
#include "camera.h"
#include "Object.h"
#include "ShaderManager.h"
class Scene
{
public:
    Scene(std::vector<Object> objects, glm::vec3 cameraPos);
    void setShaderCount() const;

    Scene() = delete;

    std::vector<Object> objects;

    Camera camera;

    void Init();

    void Update(double timeDelta);

    void AddAll(const std::vector<Object>& objects);

    void AddObject(const Object& object);

    void removeObject(size_t id);


    size_t indexOf(unsigned int objectId);

    class Builder{

        std::vector<Object> objects;
        glm::vec3 cameraPos { 0.f };

        void reset();

    public:

        Builder();

        Builder& setCameraPosition(glm::vec3 position);
        Builder& setCameraPosition(float x, float y, float z);

        Builder& addAll(const std::vector<Object>& objects);
        Builder& addObject(const Object& object);


        Scene* build();
    };
};


