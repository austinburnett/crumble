#version 330 core

in vec3 particleColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(particleColor, 1.0f);
    //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
