#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;
layout (location = 3) in vec3 Tangent;

out vec4 worldPos;
out vec2 uv;
out vec4 n;
out vec4 tangent;
out vec4 biTangent;
uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJECTION;
uniform vec2 tile;


void main()
{
    uv = TexCoords * tile;
	worldPos = MODEL * vec4(pos, 1.0);
	n = MODEL * vec4(Normal, 0.0);
	tangent = MODEL * vec4(Tangent, 0.0);
	biTangent = MODEL * vec4(cross(Normal, Tangent).xyz, 0.0);
    gl_Position = PROJECTION * VIEW * MODEL * vec4(pos, 1.0);
}