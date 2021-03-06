#version 150 core

uniform float iTime;
uniform sampler2D iChannel0;
in vec2 fragCoord;
out vec4 fragColor;

void main() {
    vec2 uv = fragCoord;
    vec2 yo = uv * 2. - 1.;
    vec3 dir = vec3(yo, 1);
    vec3 camera = vec3(0, 1, iTime);
    vec3 p = camera + dir * camera.y / abs(dir.y);
    vec3 color =  vec3(texture(iChannel0, p.xz));
    fragColor = vec4(color, 1);
}