#version 400
out vec4 frag_colour;
uniform vec3 cameraPosition;

in vec3 color;
in vec4 ex_worldPosition;
in vec3 ex_worldNormal;

void main ()
{
    const vec3 ambient = vec3( 0.1, 0.1, 0.1);
    const float specularStrength = 1;
    vec3 lightPosition= vec3(0.0,0.0,0.0);
    float dist = length(lightPosition - ex_worldPosition.xyz);
    float attenuation = clamp(5.0 / dist, 0.0, 1.0);

    vec3 lightDirection = normalize(lightPosition - vec3(ex_worldPosition));
    vec3 viewDirection = normalize(cameraPosition - vec3(ex_worldPosition));
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);

    float dot_product = dot(lightDirection, ex_worldNormal);
    vec3 diffuse = max(dot_product, 0.0) * color * attenuation;

    float specValue = pow( max(dot(viewDirection, halfwayDirection), 0.0), 32.0);
    vec3 spec = specularStrength * specValue * vec3( 1.0, 1.0, 1.0);

    vec3 specular = spec * attenuation;
    frag_colour = vec4((ambient + diffuse + specular ), 1.0) * vec4(color, 1.0);
}