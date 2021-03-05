#version 150 core

uniform float iTime;
uniform sampler2D iChannel0;
in vec2 fragCoord;
out vec4 fragColor;

void main() {
    vec2 p = -1.0 + 2.0 * fragCoord.xy;
    float r = sqrt( dot(p,p) );
    float a = atan(p.y,p.x);
    vec2 uv;
    uv.x = .1 * iTime + .1 / r;
    uv.y = 1. * a / 3.1416;
    vec3 col = texture(iChannel0,uv).xyz;
    col *= smoothstep(0., .75, r);
    fragColor = vec4(col,1.0);
}
