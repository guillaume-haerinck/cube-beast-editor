R"(
#version 300 es
layout(location = 0) out lowp vec4 color;

// Default precision qualifier
precision highp float;

// This represents the current texture on the mesh
uniform lowp sampler2D texture;

// The interpolated texture coordinate for this fragment
varying highp vec2 v_texCoord;

void main() {
    lowp vec4 col = texture2D(texture, v_texCoord);

    float x = fract(v_texCoord.x * 5.0);
    float y = fract(v_texCoord.y * 5.0);

    // Draw a black and white grid.
    if (x > 0.9 || y > 0.9) {
        color = vec4(1,1,1,1);
    } else {
        discard;
    }
}

)"
