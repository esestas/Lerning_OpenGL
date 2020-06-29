#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texTest;

	

// uniform float near_plane;
// uniform float far_plane;

//required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * 0.1 * 100) / (100 + 0.1 - z * (100 - 0.1));	
}

void main()
{             
    float depthValue = texture(texTest, TexCoords).r;
     //FragColor = vec4(vec3(LinearizeDepth(depthValue) / 1000), 1.0); // perspective
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}
