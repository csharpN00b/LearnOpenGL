#version 330 core

out vec4 FragColor;
uniform float offset;

void main()
{
	FragColor = vec4(1.0f, -offset, offset, 1.0f);
}