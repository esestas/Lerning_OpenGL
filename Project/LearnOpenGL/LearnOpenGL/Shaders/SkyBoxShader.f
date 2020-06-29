#version 330 core

out vec4 FragColor;
in vec3 TexCoords;
uniform samplerCube  cubeMap;

void main()
{
	vec3 col = texture(cubeMap, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
} 