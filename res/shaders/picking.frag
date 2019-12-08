#version 300 es
layout(location = 0) out lowp uint color;

flat in lowp uint id;

void main() {
	color = id;
}
