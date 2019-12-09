R"(
#version 300 es
layout(location = 0) out lowp float color;

in lowp float id;

void main() {
	color = id;
}

)"
