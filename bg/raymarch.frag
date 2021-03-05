#version 150 core

in vec2 fragCoord;
out vec4 fragColor;
uniform float iTime;
uniform sampler2D iChannel0;
uniform vec2 iResolution;

float map_the_world(vec3 p) {
    float displacement = sin(p.x * 5 + iTime * 1.7) * sin(p.y * 7  + iTime * 0.4) * sin(p.z * 11  + iTime * 1.1) * .05;
    return length(p) - 1.0 + displacement;
}

vec3 calculate_normal(vec3 p) {
    const vec3 small_step = vec3(0.001, 0.0, 0.0);
    float gradient_x = map_the_world(p + small_step.xyy) - map_the_world(p - small_step.xyy);
    float gradient_y = map_the_world(p + small_step.yxy) - map_the_world(p - small_step.yxy);
    float gradient_z = map_the_world(p + small_step.yyx) - map_the_world(p - small_step.yyx);
    vec3 normal = vec3(gradient_x, gradient_y, gradient_z);
    return normalize(normal);
}

vec3 ray_march(vec3 ro, vec3 rd) {
    float total_distance_traveled = 0;
    for (int i = 0; i < 64; ++i) {
        vec3 current_position = ro + total_distance_traveled * rd;
        float distance_to_closest = map_the_world(current_position);
        if (distance_to_closest < 0.001) {
            return calculate_normal(current_position);
        }
        total_distance_traveled += distance_to_closest;
    }
    return vec3(0);
}

void main() {
    vec3 ro = vec3(0, 0, -1.5);
    vec3 rd = vec3((fragCoord * 2 - 1) * vec2(iResolution.x / iResolution.y, 1), 1);
    vec3 normal = ray_march(ro, rd);
    vec3 color = texture(iChannel0, normal.xy * .5 + .5).rgb;
    fragColor = vec4(color, 1);
}
