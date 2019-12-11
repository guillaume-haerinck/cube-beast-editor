R"(
#version 300 es
layout(location = 0) out lowp vec3 color;

in lowp vec3 id;

void main() {
	color = id;
}

)"
