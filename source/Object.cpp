//
// Created by Pavel Janu(Skola) on 10/20/2023.
//
#include "Object.h"
#include "Scene.h"


unsigned int Object::objectCount = 0;

const glm::vec3 Object::defaultColor{ 1, 0, 0 };
const glm::vec3 Object::secondaryColor{ 0.15, 0.89, 0.68 };

void Object::rotate(const glm::vec3 radians) {
    rotation = radians;
}

void Object::move(glm::vec3 delta) {
    translation += delta;
}

void Object::scale(const glm::vec3 newScales) {
    scales = newScales;
}
glm::mat4 Object::transformation() const
{
    if (composite->tranformations.empty())
    {
        composite->AddTransformation(translateMat);
        composite->AddTransformation(scaleMat);
        composite->AddTransformation(rotateMatX);
        composite->AddTransformation(rotateMatY);
        composite->AddTransformation(rotateMatZ);
    }
    translateMat->move = translation;
    scaleMat->scales = scales;
    rotateMatX->radians = rotation.x;
    rotateMatX->radians = rotation.y;
    rotateMatX->radians = rotation.z;

    return composite->Calculate(glm::mat4 { 1.f });
}

float Object::getAcc(Direction dir) {
    return acceleration * (int)dir;
}

void Object::applyFy(Direction dir) {
    movementDir.y = getAcc(dir);
}

void Object::applyFx(Direction dir) {
    movementDir.x = getAcc(dir);
}


void Object::enableGrowth(Scale direction) {
    scaleDr = direction;
}
float Object::calcNewV(float current, const float acc, const float dec) {

    if (std::abs(current) <= dec) {
        current = 0;
    } else {
        current -= (-1 + (2 * (current > 0))) * dec;
    }

    return current + acc;
}
void Object::disableGrowth() {
    scaleDr = Scale::none;
}

void Object::enableRotation(Rotation direction) {
    rotateDr = direction;
}

void Object::disableRotation() {
    rotateDr = Rotation::none;
}

float Object::capV(float current, float max) {
    return std::min(std::max(current, -max), max);
}

void Object::updateGrowth(const float dt) {
    const float delta = growthV * (int)scaleDr * dt;
    scale({ scales.x + delta, scales.y + delta, scales.z + delta });
}

void Object::updateRotation(const float dt) {

    const float dec = rotationDec * dt;
    const float acc = rotationAcc * dt * (int)rotateDr;

    rotationV = capV(calcNewV(rotationV, acc, dec), rotationMax);

    rotation.y += rotationV * dt;
}

void Object::updateForces(const float dt) {

    const float dec = deceleration * dt;
    const float acc = acceleration * dt;

    const float fx = capV(calcNewV(forces.x, acc * movementDir.x, dec), maxV);
    const float fy = capV(calcNewV(forces.y, acc * movementDir.y, dec), maxV);

    forces = { fx, fy };

    move({ fx * dt, fy * dt, 0.f });
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
    shader.get().passUniformLocation("objectColor", color);
    shader.get().passUniformLocation("modelMatrix", transformation());
    model->draw(id, shader.get());
}


void Object::update(const double dt) {
    updateGrowth(dt);
    updateRotation(dt);
    updateForces(dt);
    updateMovement(dt);
}

void Object::AddTransformation(std::shared_ptr<TransComponent> component)
{
    this->composite->AddTransformation(component);
}

void Object::updateMovement(double dt) {

    if (not movementCalculator.has_value()) {
        return;
    }

    auto & mc = *(movementCalculator.value());
    mc.update(dt);
    translation = mc.currentPosition();
    rotation = mc.currentHeading();

}

Object::Object(std::shared_ptr<Model> model, Shader & shader, std::shared_ptr<Texture> texture,
               std::shared_ptr<MovementCalculator> movementCalculator) :
        model(std::move(model)), shader(shader), texture(std::move(texture)), id(getNextId()),
        movementCalculator(movementCalculator) { }

void Object::setMovement(std::shared_ptr<MovementCalculator> mc) {
    movementCalculator.emplace(std::move(mc));
}

void Object::Builder::reset()
{
    model = nullptr;
    shader = nullptr;
    texture = nullptr;
    movementCalculator->reset();
    rotationRadians = glm::vec3{ 0.f };
    position = glm::vec3{ 0.f };
    scales = glm::vec3{ 1.f };
    color = defaultColor;
}



Object Object::Builder::createObject()
{
    if (movementCalculator && *movementCalculator) {
        return Object { std::move(model), *shader, std::move(texture), movementCalculator.value() };
    }
    return Object { std::move(model), *shader, std::move(texture) };
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

Object::Builder& Object::Builder::emplaceObject(std::shared_ptr<Model> model, Shader& shader, std::shared_ptr<Texture> texture )
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
    obj.move(position);
    obj.scale(scales);
    obj.rotate(rotationRadians);
    obj.color = color;

    reset();

    return obj;
}


Object::Builder &Object::Builder::setTexture(std::shared_ptr<Texture> texture) {
    texture = std::move(texture);
    return *this;
}

Object::Builder &Object::Builder::setRotation(glm::vec3 radians) {
    rotationRadians = radians;
    return *this;
}

Object::Builder & Object::Builder::setMovement(std::shared_ptr<MovementCalculator> mc) {
    movementCalculator.emplace(std::move(mc));
    return *this;
}

