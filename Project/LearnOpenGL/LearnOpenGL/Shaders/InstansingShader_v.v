#version 330 core
layout (location = 0) in vec2 iPos;
layout (location = 1) in vec3 iColor;
layout (location = 2) in vec2 iOffSet;


out vec3 colorI;
uniform vec2 offSets[100];
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    colorI = iColor;    
    //gl_Position = vec4(iPos + offSets[gl_InstanceID], 0.0, 1.0);
    gl_Position = vec4(iPos + iOffSet, 0.0, 1.0);
}