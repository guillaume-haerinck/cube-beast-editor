#version 300 es
layout(location = 0) out uint color;

in lowp float id;

void main() {
	color = uint(id);
}
