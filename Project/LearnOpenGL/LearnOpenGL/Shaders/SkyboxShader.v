#version 330 core
layout (location = 0) in vec3 pos;
//layout (location = 1) in vec2 TexCoords;

out vec3 TexCoords;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = pos.xyz;
    gl_Position = (projection * view * vec4(pos, 1.0)).xyww;
}