#version 400
out vec4 frag_colour;
#define MAX_LIGHTS 100

struct Light
{
    vec3 position;
    vec3 direction;
    float cutoff;
    vec3 lightColor;
    float outerCutoff;
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
    vec3 lightDir = normalize(-light.direction);
    float dot_product = dot(lightDir, normal);
    vec3 diffuse = max(dot_product, 0.0) * light.lightColor;

    vec3 reflectionDir = reflect(-lightDir, normal);
    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), material.shininess);
    vec3 spec = specValue * light.lightColor * material.specular;

    if (dot_product < 0.0) {
        spec = vec3(0.0);
    }

    return (diffuse + spec) * color;
}

vec3 calculatePointLight(Light light, vec3 normal, vec3 viewDir, vec3 color)
{
    vec3 worldPosition = vec3(ex_worldPosition);
    vec3 lightDir = normalize(light.position - worldPosition);
    float dist = length(light.position - worldPosition);
    float attenuation = clamp(5.0 / dist, 0.0, 1.0);

    float dot_product = dot(lightDir, normal);
    vec3 diffuse = max(dot_product, 0.0) * attenuation * material.diffuse;

    vec3 reflectionDir = reflect(-lightDir, normal);
    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), material.shininess);
    vec3 spec = specValue * light.lightColor * material.specular;

    if (dot_product < 0.0) {
        spec = vec3(0.0);
    }

    return (diffuse + spec) * color;
}

vec3 calculateSpotLight(Light light, vec3 normal, vec3 viewDir, vec3 color)
{
    vec3 worldPosition = vec3(ex_worldPosition);
    vec3 lightDir = normalize(light.position - worldPosition);
    float dist = length(light.position - worldPosition);
    float attenuation = clamp(5.0 / dist, 0.0, 1.0);

    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = smoothstep(light.outerCutoff, light.cutoff, theta);
    if (theta <= light.cutoff) {
        return vec3(0.0);
    }

    float dot_product = dot(lightDir, normal);
    vec3 diffuse = max(dot_product, 0.0) * attenuation * material.diffuse * intensity;

    vec3 reflectionDir = reflect(-lightDir, normal);
    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), material.shininess);
    vec3 spec = specValue * light.lightColor * material.specular * intensity;

    if (dot_product < 0.0) {
        spec = vec3(0.0);
    }

    return (diffuse + spec) * color;
}


void main ()
{
    vec3 fragColor = vec3(0.0, 0.0, 0.0);
    vec3 worldNormal = normalize(ex_worldNormal);
    vec3 worldPosition = vec3(ex_worldPosition);
    //ambientColor = material.ambient;
    vec4 tex = texture(textureUnitID, uv);
    vec3 color = vec3(tex.x, tex.y, tex.z);
    for (int i = 0; i < lightCount; i++)
    {
        vec3 lightColor = lights[i].lightColor;
        vec3 lightPosition = lights[i].position;
        vec3 lightDirection = lights[i].direction;
        float cutoff = lights[i].cutoff;

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