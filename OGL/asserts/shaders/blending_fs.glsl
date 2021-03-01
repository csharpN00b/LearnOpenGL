#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
	// While discarding fragments is great and all, it doesn't give us the flexibility to render semi-transparent images; 
	// we either render the fragment or completely discard it
	//vec4 texColor = texture(texture1, TexCoords);
	//if(texColor.a < 0.1)
	//	discard;
	//FragColor = texColor;

	// blending
	FragColor = texture(texture1, TexCoords);
}