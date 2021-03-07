#version 150 core

in vec2 fragCoord;
out vec4 fragColor;
uniform float iTime;
uniform sampler2D iChannel0;
uniform vec2 iResolution;

float map(vec3 p) {
    float s = iTime * .1;
    float displacement = sin(p.x * 5 + s * 1.7) * sin(p.y * 7  + s * 0.4) * sin(p.z * 11  + s * 1.1) * .05;
    return length(p) - 1.0 + displacement;
}

vec3 calculate_normal(vec3 pos) {
    vec2 e = vec2(1.0,-1.0) * 0.5773 * 0.0005;
    return normalize(e.xyy * map(pos + e.xyy) + e.yyx * map(pos + e.yyx) + e.yxy * map(pos + e.yxy) + e.xxx * map(pos + e.xxx));
}

vec3 ray_march(vec3 ro, vec3 rd) {
    float total_distance_traveled = 0;
    for (int i = 0; i < 64; ++i) {
        vec3 current_position = ro + total_distance_traveled * rd;
        float distance_to_closest = map(current_position);
        if (distance_to_closest < 0.001) {
            return calculate_normal(current_position);
        }
        total_distance_traveled += distance_to_closest;
    }
    return vec3(0);
}

void main() {
    vec3 ro = vec3(cos(iTime * .025), sin(iTime * .025) * .5, -1);
    vec3 rd = vec3((fragCoord * 2 - 1) * vec2(iResolution.x / iResolution.y, 1), 1);
    vec3 normal = ray_march(ro, rd);
    vec3 color = texture(iChannel0, normal.xy * .5 + .5).rgb;
    fragColor = vec4(color * length(normal), 1);
}
