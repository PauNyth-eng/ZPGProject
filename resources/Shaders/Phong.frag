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
            vec3 diffuse = max(dot_product, 0.0) * lightColor * attenuation;

            float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
            vec3 spec = specularStrength * specValue * lightColor;
            if (dot_product < 0.0) {
                spec = vec3(0.0);
            }

            vec3 specular = spec * attenuation;

            fragColor += (diffuse + specular) * color;

        }
    }

    frag_colour = vec4(fragColor + ambientColor, 1.0);



//    const vec3 ambient = vec3( 0.1, 0.1, 0.1);
//    const float specularStrength = 1;
//    vec3 lightPosition= vec3(0.0,0.0,0.0);
//    float dist = length(lightPosition - ex_worldPosition.xyz);
//    float attenuation = clamp(5.0 / dist, 0.0, 1.0);
//
//    vec3 lightDirection = normalize(lightPosition - vec3(ex_worldPosition));
//    vec3 viewDirection = normalize(cameraPosition - vec3(ex_worldPosition));
//    vec3 reflectDirection = reflect (-lightDirection , ex_worldNormal );
//
//    float dot_product = dot(lightDirection, ex_worldNormal);
//    vec3 diffuse = max(dot_product, 0.0) * color * attenuation;
//
//    float specValue = pow( max(dot(viewDirection, reflectDirection), 0.0), 32.0);
//    vec3 spec = specularStrength * specValue * vec3( 1.0, 1.0, 1.0);
//
//    vec3 specular = spec * attenuation;
//    frag_colour = vec4((ambient + diffuse + specular ), 1.0) * vec4(color, 1.0);
}