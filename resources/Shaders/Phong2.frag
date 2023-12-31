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



in vec3 color;
in vec4 ex_worldPosition;
in vec3 ex_worldNormal;
uniform vec3 cameraPosition;
uniform Light lights[MAX_LIGHTS];
uniform vec3 ambientColor;
uniform int lightCount;
void main ()
{
    vec3 fragColor = vec3(0.0, 0.0, 0.0);

    vec3 worldNormal = normalize(ex_worldNormal);
    vec3 worldPosition = vec3(ex_worldPosition);

    for (int i = 0; i < lightCount; i++)
    {
        vec3 lightColor = lights[i].lightColor;
        vec3 lightPosition = lights[i].position;
        vec3 lightDirection = lights[i].direction;
        float cutoff = lights[i].cutoff;

        if (lights[i].lightType == 0)
        {
            const float specularStrength = 0.3;


            vec3 viewDir = normalize(cameraPosition - worldPosition);
            vec3 lightDir = normalize(lightPosition - worldPosition);
            vec3 reflectionDir = reflect(-lightDir, worldNormal);

            float dot_product = dot(lightDir, worldNormal);
            vec3 diffuse = max(dot_product, 0.0) * lightColor;

            float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 1);
            vec3 spec = specularStrength * specValue * lightColor;


            fragColor += (diffuse + spec) * color;
        }
        else if (lights[i].lightType == 1)
        {
            const float specularStrength = 0.3;

            float dist = length(lightPosition - worldPosition);
            float attenuation = clamp(5.0 / dist, 0.0, 1.0);

            vec3 viewDir = normalize(cameraPosition - worldPosition);
            vec3 lightDir = normalize(lightPosition - worldPosition);
            vec3 reflectionDir = reflect(-lightDir, worldNormal);

            float dot_product = dot(lightDir, worldNormal);
            vec3 diffuse = max(dot_product, 0.0) * lightColor * attenuation;

            float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 1);
            vec3 spec = specularStrength * specValue * lightColor;

            vec3 specular = spec * attenuation;

            fragColor += (diffuse + specular) * color;

        }
    }

    frag_colour = vec4(fragColor + ambientColor, 1.0);

}