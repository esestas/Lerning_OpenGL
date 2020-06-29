#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 TexCoords;

out vec2 uv;

uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJECTION;
uniform vec2 tile;

void main()
{
    uv = TexCoords * tile;    
    gl_Position = PROJECTION * VIEW * MODEL * vec4(pos, 1.0);
}