#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;

out vec2 uv;
out vec3 aNormal;
out vec3 position;;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	position = vec3(model * vec4(pos, 1.0));
    uv = TexCoords;    
    aNormal = mat3(transpose(inverse(model))) * Normal;    
    gl_Position = projection * view * model * vec4(pos, 1.0);
}