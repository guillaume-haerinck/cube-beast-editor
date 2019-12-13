R"(#version 300 es
layout(location = 0) out lowp vec4 color;

precision highp float;

uniform lowp sampler2D u_texture;

in lowp vec2 v_texCoord;

void main() {
    lowp vec4 col = texture(u_texture, v_texCoord);

    float x = fract(v_texCoord.x * 10.0);
    float y = fract(v_texCoord.y * 10.0);

    if (x > 0.95 || y > 0.95) {
        color = vec4(1, 1, 1, 1);
    } else {
        discard;
    }
}

)"
