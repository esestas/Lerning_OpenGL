#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 TexCoords;

out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    uv = TexCoords;    
    gl_Position = projection * view * model * vec4(pos, 1.0);
}