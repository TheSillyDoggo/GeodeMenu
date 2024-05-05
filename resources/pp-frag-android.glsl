#version 300 es

precision highp float;

in vec2 TexCoords;

uniform sampler2D screen;
uniform vec2 screenSize;
uniform bool fast;
uniform bool first;
uniform float radius;

out vec4 FragColor;

void main() {
    FragColor = vec4(1, 0, 0, 1)
}
