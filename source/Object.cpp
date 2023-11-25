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

Object::Object(std::shared_ptr<Model> model, Shader& shader, std::shared_ptr<Texture> texture) : model(std::move(model)), shader(shader), id(getNextId()), composite(std::make_shared<TransComposite>()), texture(std::move(texture))
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
    updateScale(dt);
    updateRotate(dt);
    updateMove(dt);
}

void Object::AddTransformation(TransComponent* component)
{
    this->composite->AddTransformation(component);
}


void Object::updateScale(float d) {
    const float del = 0.1f * (int)scaleDr * d;
}

void Object::updateRotate(float d) {

    if (rotationRadians.x == 0 && rotationRadians.y == 0 && rotationRadians.z == 0) {
        return;
    }

    float acc = 1.f * (int)rotateDr * d;

    // Calculate the position relative to the rotation center (rotationCenter is set to (0,0,0))
    glm::vec3 relativePosition = position - rotationCenter;

    // Calculate the rotation matrix based on the rotation axis (e.g., glm::vec3(0, 1, 0))
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(acc), direction);

    // Update the relative position by applying the rotation matrix
    relativePosition = glm::vec3(rotationMatrix * glm::vec4(relativePosition, 1.0f));

    // Update the object's position by adding the rotation center back to the relative position (rotationCenter is set to (0,0,0))
    position = relativePosition + rotationCenter;

    // Update the object's transformation
    setRotate(glm::vec3{acc}, direction, rotationCenter);
}

void Object::updateMove(float d) {

    const float accx = 1.f * d * movementDir.x;
    const float accy = 1.f * d * movementDir.y;

    //this->position += glm::vec3{ accx, accy, 0.f };

}

void Object::setScaleDr(Scale scaleDr) {
    this->scaleDr = scaleDr;
}

void Object::setRotateDr(Rotation rotateDr) {
    this->rotateDr = rotateDr;
}

void Object::setScale(glm::vec3 scales) {
    AddTransformation(new TransScale(scales));
}



void Object::setMove(glm::vec3 position) {
    this->position += position;
    AddTransformation(new TransTranslate(position));
}

void Object::setRotate(glm::vec3 radians, glm::vec3 direction, glm::vec3 rotationCenter, float radius) {
    if (radians == glm::vec3{ 0.f }) {
        return;
    }
    this->radius = radius;
    this->rotationRadians = radians;
    this->direction = direction;
    this->rotationCenter = rotationCenter;
    AddTransformation(new TransRotate(rotationRadians.x + rotationRadians.y + rotationRadians.z, direction, rotationCenter, radius));
}

void Object::Builder::reset()
{
    model = nullptr;
    shader = nullptr;
    texture = nullptr;
    rotationRadians = glm::vec3{ 0.f };
    position = glm::vec3{ 0.f };
    scales = glm::vec3{ 1.f };
    color = defaultColor;
}



Object Object::Builder::createObject()
{
    return Object{ std::move(model), *shader , std::move(texture)};
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

Object::Builder& Object::Builder::emplaceObject(std::shared_ptr<Model> model, Shader& shader, std::shared_ptr<Texture> texture)
{
    return setModel(std::move(model)).setTexture(std::move(texture)).setShader(shader);
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
    obj.setMove(position);
    obj.setScale(scales);
    obj.setRotate(rotationRadians, direction, rotationCenter, radius);
    obj.color = color;

    reset();

    return obj;
}

Object::Builder &Object::Builder::setRotation(glm::vec3 radians, glm::vec3 direction, glm::vec3 rotationCenter, float radius) {
    this->rotationRadians = radians;
    this->direction = direction;
    this->rotationCenter = rotationCenter;
    this->radius = radius;
    return *this;
}

Object::Builder &Object::Builder::setTexture(std::shared_ptr<Texture> texture) {
    texture = std::move(texture);
    return *this;
}

