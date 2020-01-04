R"(#version 300 es
precision lowp float;
layout(location = 0) out lowp vec4 color;

uniform sampler2D u_texture;

in vec2 v_texCoord;

void main() {
    vec4 col = texture(u_texture, v_texCoord);

    float x = fract(v_texCoord.x * 20.0);
    float y = fract(v_texCoord.y * 20.0);

    if (x > 0.95 || y > 0.95) {
        color = vec4(1, 1, 1, 1);
    } else {
        discard;
    }
}

)"
