#version 300 es
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

uniform mat4x4 mvp;

void main() {
    gl_Position = mvp * vPosition;
}