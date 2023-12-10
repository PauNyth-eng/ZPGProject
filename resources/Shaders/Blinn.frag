#version 400
out vec4 frag_colour;
#define MAX_LIGHTS 100

struct Light
{
    vec3 position;
    vec3 direction;
    float cutoff;
    vec3 lightColor;
    int lightType;
};

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int shininess;
};


in vec3 color;
in vec4 ex_worldPosition;
in vec3 ex_worldNormal;
in vec2 uv;


uniform Material material;
uniform vec3 cameraPosition;
uniform Light lights[MAX_LIGHTS];
uniform vec3 ambientColor;
uniform int lightCount;
uniform sampler2D textureUnitID;

vec3 calculateDirectionalLight(Light light, vec3 normal, vec3 viewDir, vec3 color)
{
    const float specularStrength = 0.4;

    vec3 lightDir = normalize(-light.direction);
    float dot_product = dot(lightDir, normal);
    vec3 diffuse = max(dot_product, 0.0) *light.lightColor;

    vec3 halfwayDirection = normalize(lightDir + viewDir);

    float specValue = pow( max(dot(viewDir, halfwayDirection), 0.0), 32.0);

    vec3 spec = specularStrength * specValue * light.lightColor;
    if (dot_product < 0.0)
    {
        spec = vec3(0.0, 0.0, 0.0);
    }

    return (diffuse + spec) * color;
}


vec3 calculatePointLight(Light light, vec3 normal, vec3 viewDir, vec3 color)
{
    const float specularStrength = 0.4;

    vec3 lightDir = normalize(light.position - ex_worldPosition.xyz);
    float dot_product = dot(lightDir, normal);
    vec3 diffuse = max(dot_product, 0.0) *light.lightColor;

    vec3 halfwayDirection = normalize(lightDir + viewDir);

    float specValue = pow( max(dot(viewDir, halfwayDirection), 0.0), 32.0);

    vec3 spec = specularStrength * specValue * light.lightColor;
    if (dot_product < 0.0)
    {
        spec = vec3(0.0, 0.0, 0.0);
    }

    return (diffuse + spec) * color;
}

vec3 calculateSpotLight(Light light, vec3 normal, vec3 viewDir, vec3 color)
{
    const float specularStrength = 0.4;

    vec3 lightDir = normalize(light.position - ex_worldPosition.xyz);
    float dot_product = dot(lightDir, normal);
    vec3 diffuse = max(dot_product, 0.0) *light.lightColor;

    vec3 halfwayDirection = normalize(lightDir + viewDir);

    float specValue = pow( max(dot(viewDir, halfwayDirection), 0.0), 32.0);

    vec3 spec = specularStrength * specValue * light.lightColor;
    if (dot_product < 0.0)
    {
        spec = vec3(0.0, 0.0, 0.0);
    }

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff;
    float intensity = clamp((theta - epsilon) / (1 - epsilon), 0, 1);

    return (diffuse + spec) * color * intensity;
}
void main ()
{
    vec3 fragColor = vec3(0.0, 0.0, 0.0);
    vec3 worldNormal = normalize(ex_worldNormal);
    vec3 worldPosition = vec3(ex_worldPosition);
    vec4 tex = texture(textureUnitID, uv);
    vec3 color = vec3(tex.x, tex.y, tex.z);
    for (int i = 0; i < lightCount; i++)
    {

        if (lights[i].lightType == 0)
        {
            vec3 viewDir = normalize(cameraPosition - worldPosition);

            fragColor += calculateDirectionalLight(lights[i], worldNormal, viewDir, color);
        }
        else if (lights[i].lightType == 1)
        {
            vec3 viewDir = normalize(cameraPosition - worldPosition);
            fragColor += calculatePointLight(lights[i], worldNormal, viewDir, color);

        }
        else if (lights[i].lightType == 2)
        {

            vec3 viewDir = normalize(cameraPosition - worldPosition);
            fragColor += calculateSpotLight(lights[i], worldNormal, viewDir, color);

        }


    }


    frag_colour = vec4(fragColor * material.ambient, 1.0);
}