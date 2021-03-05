#type vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	Color = aColor;
}


#type fragment
#version 330 core
out vec4 FragColor;

in vec3 Color;

void main()
{
	FragColor = vec4(Color, 1.0f);
}