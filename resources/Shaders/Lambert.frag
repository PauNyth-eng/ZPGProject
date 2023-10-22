#version 400
out vec4 frag_colour;

in vec3 color;
in vec4 ex_worldPosition;
in vec3 ex_worldNormal;
void main ()
{
    vec3 lightPosition= vec3(0.0,0.0,0.0);
    vec3 lightDirection = normalize(lightPosition - vec3(ex_worldPosition));
    float lightIntensity = max(dot(normalize(lightDirection), normalize(ex_worldNormal)), 0.0);
    vec4 diffuse = lightIntensity * vec4( 0.385, 0.647, 0.812, 1.0);
    vec4 ambient = vec4( 0.1, 0.1, 0.1, 1.0);
    frag_colour = (ambient + diffuse) * vec4(color, 1.0);
}