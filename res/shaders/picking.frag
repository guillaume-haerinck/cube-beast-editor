#version 300 es
layout(location = 0) out lowp vec3 color;

in lowp vec3 myValue;

void main() {
	color = myValue;
}
