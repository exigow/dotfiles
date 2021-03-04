#version 150 core

uniform float iTime;
uniform sampler2D texSampler;
in vec2 fragCoord;
out vec4 fragColor;

vec2 rotate_position(vec2 pos, vec2 centre, float angle) {
    float sinAngle = sin(angle);
    float cosAngle = cos(angle);
    pos -= centre;
    vec2 rotatedPos;
    rotatedPos.x = pos.x * cosAngle - pos.y * sinAngle;
    rotatedPos.y = pos.x * sinAngle + pos.y * cosAngle;
    rotatedPos += centre;
    return rotatedPos;
}

vec3 pal(float t, vec3 a, vec3 b, vec3 c, vec3 d) {
    return a + b * cos(6.28318 * (c * t + d));
}

void main() {
    float amplitude = 0.9;
    float frequency = 0.4;
    vec2 centre = vec2(0.5);
    vec2 uv = rotate_position(fragCoord, centre, iTime * 0.025);
    uv.y -= iTime * 0.1;
    vec2 position = uv * 4;
    position.x += amplitude * sin(frequency * position.y);
    float value = fract(position.x);
    float blocky = floor(value * 4) / 4;
    vec3 color = pal(blocky, vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(2.0, 1.0, 0.0), vec3(0.5, 0.20, 0.25));
    vec2 vig_uv = fragCoord.xy * (1.0 - fragCoord.xy);
    float vig = pow(vig_uv.x * vig_uv.y * 15.0, 0.25);
    vec4 tex = texture(texSampler, fragCoord);
    fragColor = vec4(tex.rgb * color * vig, 1.0);
}
