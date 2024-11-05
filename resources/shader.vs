#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTranslation;
layout (location = 2) in vec3 aColor;

out vec3 particleColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = vec4(aTranslation + aPos, 1.0f);
    particleColor = aColor;
    //gl_Position = model * vec4(aPos, 1.0f);
    //gl_Position = proj * view * model * vec4(aPos, 1.0f);
}
