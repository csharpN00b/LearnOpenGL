#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = aNormal;
	//Normal = mat3(transpose(inverse(model))) * aNormal; // better to calculate the normal matrix on the CPU and send it to the shaders via a uniform before drawing 

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}