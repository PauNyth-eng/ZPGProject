#version 400

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
//uniform vec3 objectColor;


out vec3 color;
out vec4 ex_worldPosition;
out vec3 ex_worldNormal;


layout(location=0) in vec3 positionVec;
layout(location=1) in vec3 normalVec;
layout(location=2) in vec2 in_uv;

void main ()
{
    ex_worldPosition = modelMatrix * vec4(positionVec, 1.0);
    ex_worldNormal =  normalize(transpose(inverse(mat3(modelMatrix))) * normalVec);
    color = vec3 (1 ,1 ,1 );
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4 (positionVec, 1.0);
}