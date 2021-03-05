#version 150 core

uniform float iTime;
uniform sampler2D iChannel0;
in vec2 fragCoord;
out vec4 fragColor;

vec3 marchNotTheMonth(vec3 s, vec3 d)
{
    float h = s.y;
    float a = -d.y;
    float t = h/a;
    vec3 p = s+d*t;
    vec2 texUv = p.xz;
    return vec3(texture(iChannel0,texUv));
}

void main() {
    vec2 uv = fragCoord;
    vec2 yo = uv * 2. - 1.;
    vec3 dir = vec3(yo,1);
    vec3 camera = vec3(1, 1, iTime);
    vec3 march =  marchNotTheMonth(camera, dir);
    vec3 col = vec3(march);
    fragColor = vec4(col, 1.0);
}