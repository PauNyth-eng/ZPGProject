#version 400

out vec4 out_color;

in vec3 texCoords;

uniform samplerCube textureUnitID;

void main() {
    out_color = texture(textureUnitID, texCoords);
}