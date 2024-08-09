#version 330 core

in vec2 TexCoord;

uniform sampler2D textureWood;
uniform sampler2D textureFace;

out vec4 FragColor;

void main() {
    FragColor = mix(texture(textureWood, TexCoord),
                    texture(textureFace, TexCoord), 0.2);
}
