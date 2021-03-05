#version 150 core

in vec2 fragCoord;
out vec4 fragColor;

void main() {
    fragColor = vec4(fragCoord.xy, 0, 1);
}
