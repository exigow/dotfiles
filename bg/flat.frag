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

    float h = camera.y;
    float a = -dir.y;
    float t = h / a;
    vec3 p = camera + dir * t;
    vec2 texUv = p.xz;
    vec3 color =  vec3(texture(iChannel0, texUv));
    fragColor = vec4(color, 1);
}