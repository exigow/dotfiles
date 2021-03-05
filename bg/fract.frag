#version 150 core

uniform float iTime;
uniform sampler2D iChannel0;
in vec2 fragCoord;
out vec4 fragColor;

void main() {
    vec2 u = (fragCoord * 2. - vec2(1));
    vec3 p = vec3(1, 1, iTime);
    p += vec3(u, 1) * (1. - fract(p.z));
    while (dot(sin(p), vec3(2)) > 0.) {
        p += vec3(u, 1);
    }
    fragColor = clamp(dot(sin(p - vec3(0, 0, 1)), vec3(2)), .3, 1.) * texture(iChannel0, p.xy);
}
