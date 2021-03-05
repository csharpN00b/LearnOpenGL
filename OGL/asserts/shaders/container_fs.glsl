#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(texture1, TexCoords), texture(texture2, TexCoords), 0.2);
	//FragColor = vec4(TexCoords, 1.0, 1.0);
	//FragColor = texture(texture2, TexCoords);
}