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


vec3 calculateDirectionalLight(Light light, vec3 normal, vec3 worldPosition, vec3 color)
{
    vec3 lightDir = normalize(-light.direction);
    float dot_product = max(dot(lightDir, normal), 0.0) ;

    return dot_product * color * light.lightColor;
}

vec3 calculatePointLight(Light light, vec3 normal, vec3 worldPosition, vec3 color)
{
    float dist = length(light.position - worldPosition);
    float attenuation = clamp(5.0 / dist, 0.0, 1.0);

    vec3 lightDir = normalize(light.position - worldPosition);
    float dot_product = max(dot(normal, lightDir), 0.0);

    return attenuation * dot_product * color;
}

vec3 calculateSpotLight(Light light, vec3 normal, vec3 worldPosition, vec3 color)
{
    vec3 lightDir = normalize(light.position - worldPosition);

    float theta = dot(lightDir, normalize(-light.direction));
    if(theta <= light.cutoff)
    {
        return vec3(0.0, 0.0, 0.0);
    }

    float dist = length(light.position - worldPosition);
    float attenuation = clamp(5.0 / dist, 0.0, 1.0);

    float dot_product = max(dot(normal, lightDir), 0.0);

    float epsilon = light.cutoff;
    float intensity = clamp((theta - epsilon) / (1 - epsilon), 0, 1);

    return attenuation * dot_product * color * intensity;
}

void main ()
{
    vec3 fragColor = vec3(0.0, 0.0, 0.0);
    vec3 worldNormal = normalize(ex_worldNormal);
    vec3 worldPosition = (ex_worldPosition.xyz/ex_worldPosition.w);
    vec4 tex = texture(textureUnitID, uv);
    vec3 color = vec3(tex.x, tex.y, tex.z);
    for (int i = 0; i < lightCount; i++)
    {

        if (lights[i].lightType == 0)
        {
            fragColor += calculateDirectionalLight(lights[i], worldNormal, worldPosition, color);
        }
        else if (lights[i].lightType == 1)
        {
            fragColor += calculatePointLight(lights[i], worldNormal, worldPosition, color);

        }
        else if (lights[i].lightType == 2)
        {

            fragColor += calculateSpotLight(lights[i], worldNormal, worldPosition, color);

        }


    }


    frag_colour = vec4(fragColor * material.ambient, 1.0);

}