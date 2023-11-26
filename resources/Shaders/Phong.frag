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
in vec2 uv;
uniform vec3 cameraPosition;
uniform Light lights[MAX_LIGHTS];
uniform vec3 ambientColor;
uniform int lightCount;
uniform sampler2D textureUnitID;
void main ()
{
    vec3 fragColor = vec3(0.0, 0.0, 0.0);

    vec3 worldNormal = normalize(ex_worldNormal);
    vec3 worldPosition = vec3(ex_worldPosition);

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
            const float specularStrength = 0.3;

            vec3 lightDir = normalize(-lightDirection);
            float dot_product = dot(lightDir, worldNormal);
            vec3 diffuse = max(dot_product, 0.0) * lightColor;

            vec3 viewDir = normalize(cameraPosition - worldPosition);
            vec3 reflectionDir = reflect(-lightDir, worldNormal);

            float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
            vec3 spec = specularStrength * specValue * lightColor;
            if (dot_product < 0.0) {
                spec = vec3(0.0);
            }

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
            vec3 diffuse = max(dot_product, 0.0) * color * attenuation;

            float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 1);
            vec3 spec = specularStrength * specValue * lightColor;

            if (dot_product < 0.0) {
                spec = vec3(0.0);
            }
            vec3 specular = spec * attenuation;

            fragColor += (diffuse + specular);

        }
        else if (lights[i].lightType == 2)
        {
            vec3 lightDir = normalize(lightPosition - worldPosition);

            float theta = dot(lightDirection, normalize(worldPosition - lightPosition));
            if (theta <= 0.2) {
                frag_colour += vec4(0.0);
                continue;
            }
            const float specularStrength = 0.3;

            float dist = length(lightPosition - worldPosition);
            float attenuation = clamp(5.0 / dist, 0.0, 1.0);

            vec3 viewDir = normalize(cameraPosition - worldPosition);
            vec3 reflectionDir = reflect(-lightDir, worldNormal);

            float dot_product = dot(lightDir, worldNormal);
            vec3 diffuse = max(dot_product, 0.0) * color * attenuation;

            float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 1);
            vec3 spec = specularStrength * specValue * lightColor;

            if (dot_product < 0.0) {
                spec = vec3(0.0);
            }
            vec3 specular = spec * attenuation;

            fragColor += (diffuse + specular);

        }


    }


    frag_colour = vec4(fragColor + ambientColor + ambientColor, 1.0);


}