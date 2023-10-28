//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#include "Object.h"
#include "Scene.h"


unsigned int Object::objectCount = 0;

const glm::vec3 Object::defaultColor{ 1, 0, 0 };
const glm::vec3 Object::secondaryColor{ 0.15, 0.89, 0.68 };


glm::mat4 Object::transformation() const
{
    return composite->Calculate();
}

Object::Object(std::shared_ptr<Model> model, Shader& shader) : model(std::move(model)), shader(shader), id(getNextId()), composite(std::make_shared<TransComposite>())
{

}

unsigned int Object::getNextId() {
    return ++objectCount;
}

unsigned int Object::objectId() const {
    return id;
}

void Object::setColor(glm::vec3 newColor) {
    color = newColor;
}

void Object::setColor(float r, float g, float b) {
    setColor({ r, g, b });
}


void Object::draw() const {
    shader.get().use();
    //shader.get().passUniformLocation("objectColor", color);
    shader.get().passUniformLocation("modelMatrix", transformation());

    model->draw(id, shader.get());
}

void Object::update(double dt)
{
}

void Object::AddTransformation(TransComponent* component)
{
    this->composite->AddTransformation(component);
}

void Object::Builder::reset()
{
    model = nullptr;
    shader = nullptr;

    rotationRadians = glm::vec3{ 0.f };
    position = glm::vec3{ 0.f };
    scales = glm::vec3{ 1.f };
    color = defaultColor;
}



Object Object::Builder::createObject()
{
    return Object{ std::move(model), *shader };
}

Object::Builder& Object::Builder::setModel(std::shared_ptr<Model> model)
{
    this->model = std::move(model);
    return *this;
}

Object::Builder& Object::Builder::setShader(Shader& shader)
{
    this->shader = &shader;
    return *this;
}

Object::Builder& Object::Builder::emplaceObject(std::shared_ptr<Model> model, Shader& shader)
{
    return setModel(std::move(model)).setShader(shader);
}

Object::Builder& Object::Builder::setRotation(glm::vec3 radians)
{
    rotationRadians = radians;
    return *this;
}

Object::Builder& Object::Builder::setPosition(glm::vec3 position)
{
    this->position = position;
    return *this;
}

Object::Builder& Object::Builder::setPosition(float x, float y, float z)
{
    return setPosition({ x, y, z });
}

Object::Builder& Object::Builder::setScale(glm::vec3 scales)
{
    this->scales = scales;
    return *this;
}

Object::Builder& Object::Builder::setScale(float x, float y, float z)
{
    return setScale({ x, y, z });
}

Object::Builder& Object::Builder::setColor(glm::vec3 color)
{
    this->color = color;
    return *this;
}

Object::Builder& Object::Builder::setColor(float r, float g, float b)
{
    return setColor({ r, g, b });
}

Object Object::Builder::build()
{
    if (not (model and shader)) {
        throw std::runtime_error("ObjectBuilder error: Missing value '" + std::string(model ? "shader" : "model") + "'");
    }

    Object obj = createObject();
    obj.AddTransformation(new TransTranslate(position));
    obj.AddTransformation(new TransScale(scales));
    obj.color = color;

    reset();

    return obj;
}
