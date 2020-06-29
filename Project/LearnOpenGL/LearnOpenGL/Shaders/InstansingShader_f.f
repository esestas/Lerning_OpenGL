#version 330 core
out vec4 FragColor;

in vec3 colorI;

void main()
{    

    FragColor = vec4(colorI, 1.0);
}