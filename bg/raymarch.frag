#version 150 core

in vec2 fragCoord;
out vec4 fragColor;
uniform float iTime;
uniform sampler2D iChannel0;
uniform vec2 iResolution;

float map(vec3 p) {
    float s = iTime * .1;
    float displacement = sin(p.x + s * 1.7) * sin(p.y * 1.7  + s * 0.4) * sin(p.z * 3.7 + s * 1.1) * .15;
    return length(p) - 4.0 + displacement;
}

vec3 calculate_normal(vec3 pos) {
    vec2 e = vec2(1.0,-1.0) * 0.5773 * 0.0005;
    return normalize(e.xyy * map(pos + e.xyy) + e.yyx * map(pos + e.yyx) + e.yxy * map(pos + e.yxy) + e.xxx * map(pos + e.xxx));
}

void main() {
    fragColor = vec4(0, 0, 0, 1);
    vec3 ro = vec3(cos(iTime * .025) * 2, sin(iTime * .025) * 1.5, -5);
    vec3 rd = vec3((fragCoord * 2 - 1) * vec2(iResolution.x / iResolution.y, 1), 1);
    float tmin = 1.0;
    float tmax = 4.0;
    float t = tmin;
    for (int i = 0; i < 32 && t < tmax; i++) {
        vec3 current_position = ro + tmin * rd;
        float h = map(current_position);
        if (abs(h) < 0.001 * t) {
            vec3 n = calculate_normal(current_position);
            fragColor.rgb = texture(iChannel0, n.xy * .5 + .5).rgb;
            break;
        }
        tmin += h;
    }
}
