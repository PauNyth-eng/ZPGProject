
#include "Shader.h"
#include <filesystem>
namespace fs = std::filesystem;


Shader::~Shader() {
    glDeleteProgram(shaderId);
}

void Shader::updateView(const glm::mat4& view) const
{

    passUniformLocation("viewMatrix", view);
}

void Shader::updateProjection(const glm::mat4& projection) const
{
    passUniformLocation("projectionMatrix", projection);
}

void Shader::updatePosition(const glm::vec3& position) const
{
    passUniformLocation("cameraPosition", position);
}

void Shader::passUniformLocation(const std::string& var, const glm::mat4& matrix) const
{
    passUniformLocation(var.c_str(), matrix);
}

void Shader::passUniformLocation(const char* var, const glm::mat4& matrix) const
{
    const auto model = getUniformLocation(var);
    if (model >= 0) {
        glProgramUniformMatrix4fv(shaderId, model, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

void Shader::passUniformLocation(const std::string& var, const glm::mat3& matrix) const
{
    passUniformLocation(var.c_str(), matrix);
}

void Shader::passUniformLocation(const char* var, const glm::mat3& matrix) const
{
    const auto location = getUniformLocation(var);
    if (location >= 0) {
        glProgramUniformMatrix3fv(shaderId, location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

void Shader::passUniformLocation(const std::string& var, const glm::vec3& vector) const
{
    passUniformLocation(var.c_str(), vector);
}

void Shader::passUniformLocation(const char* var, const glm::vec3& vector) const
{
    const auto location = getUniformLocation(var);
    if (location >= 0) {
        glProgramUniform3f(shaderId, location, vector.x, vector.y, vector.z);
    }
}

void Shader::passUniformLocation(const std::string& var, int32_t value) const
{
    passUniformLocation(var.c_str(), value);
}

void Shader::passUniformLocation(const char* var, int32_t value) const
{
    const auto location = getUniformLocation(var);
    if (location >= 0) {
        glProgramUniform1i(shaderId, location, value);
    }
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    compile(loadShader(vertexPath), loadShader(fragmentPath));

    if (not checkShader()) {
        throw std::runtime_error("Shader could not be compiled or linked");
    }
}

void Shader::use() const
{
    glUseProgram(shaderId);
}

bool Shader::checkShader() const {

    GLint status;
    glGetProgramiv(shaderId, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
        auto* strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(shaderId, infoLogLength, nullptr, strInfoLog);
        std::cerr << "Linker failure: " << strInfoLog << std::endl;
        delete[] strInfoLog;
    }

    return status != GL_FALSE;
}

std::string Shader::loadShader(const std::string& filename) {
    fs::path sourcePath(__FILE__);
    fs::path solutionDir = sourcePath.parent_path();

    fs::path filePath = solutionDir / filename;

    if (!fs::exists(filePath)) {
        throw std::runtime_error("Shader file not found: " + filePath.string());
    }

    std::ifstream in(filePath.string());
    std::stringstream ss;

    while (in) {
        std::string line;
        std::getline(in, line);
        ss << line << "\n";
    }

    return ss.str();
}




GLint Shader::getUniformLocation(const std::string& var) const
{
    return getUniformLocation(var.c_str());
}

GLint Shader::getUniformLocation(const char* var) const
{
    auto location = glGetUniformLocation(shaderId, var);
    if (location < 0) {
        std::cout << "Uniform variable '" << var << "' not found." << std::endl;
    }
    return location;
}

void Shader::compile(const std::string& vertex, const std::string& fragment)
{
    shaderId = glCreateProgram();

    vertexId = compileShader(vertex, GL_VERTEX_SHADER);
    fragmentId = compileShader(fragment, GL_FRAGMENT_SHADER);

    glAttachShader(shaderId, vertexId);
    glAttachShader(shaderId, fragmentId);
    glLinkProgram(shaderId);
}

GLuint Shader::compileShader(const std::string& shader, ShaderType type)
{
    GLuint id = glCreateShader(type);
    const char* shaderPtr = shader.c_str();
    glShaderSource(id, 1, &shaderPtr, nullptr);
    glCompileShader(id);
    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        GLchar  infoLog[512];
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return id;
}

void Shader::notify(EventType eventType, void* object)
{

    if ( eventType == EventType::LightChanged)
    {
        Light & light = *(Light*)object;

        applyLight(light);

        if (light.type() == LightType::Spot) {
            applyLight((SpotLight&)light);
        } else if (light.type() == LightType::Directional) {
            applyLight((DirectionalLight&)light);
        } else if (light.type() == LightType::Point) {
            applyLight((PositionedLight&)light);
        }
    }else if (eventType == EventType::CameraMoved)
    {
        Camera& camera = *((Camera*)object);
        updateView(camera.view());
        updatePosition(camera.position());
        updateProjection(camera.projection());
    }
}

void Shader::colorChanged(glm::vec3 color, size_t lightIndex, LightType lightType) const {
    if (lightType == LightType::Ambient) {
        passUniformLocation("ambientColor", color);
    } else if (lightType == LightType::Point or lightType == LightType::Directional) {
        passUniformLocation("lights[" + std::to_string(lightIndex) + "].lightColor", color);
    }
}

void Shader::positionChanged(glm::vec3 position, size_t lightIndex, LightType lightType) {
    if (lightType == LightType::Ambient) {
        return;
    } else if (lightType == LightType::Point) {
        passUniformLocation("lights[" + std::to_string(lightIndex) + "].position", position);
    } else if (lightType == LightType::Directional) {
        passUniformLocation("lights[" + std::to_string(lightIndex) + "].direction", position);
    } else if (lightType == LightType::Spot) {

        passUniformLocation("lights[" + std::to_string(lightIndex) + "].direction", position);
        passUniformLocation("lights[" + std::to_string(lightIndex) + "].position", position);
    }
}

void Shader::typeChanged(LightType type, size_t lightIndex) {
    passUniformLocation("lights[" + std::to_string(lightIndex) + "].lightType", int32_t(type));
}

void Shader::applyLight(Light &light) {
    colorChanged(light.GetColor(), light.index, light.type());
    typeChanged(light.type(), light.index);
}

void Shader::applyLight(PositionedLight & light) {
    passUniformLocation("lights[" + std::to_string(light.index) + "].position", light.GetPosition());
}

void Shader::applyLight(DirectionalLight & light) {
    passUniformLocation("lights[" + std::to_string(light.index) + "].direction", light.GetDirection());
}

void Shader::applyLight(SpotLight & light) {
    passUniformLocation("lights[" + std::to_string(light.index) + "].position", light.GetPosition());
    passUniformLocation("lights[" + std::to_string(light.index) + "].direction", light.GetPosition());
    passUniformLocation("lights[" + std::to_string(light.index) + "].cutoff", glm::cos(glm::radians(light.GetCutOff())));
}
