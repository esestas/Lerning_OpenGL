#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D tex0;
uniform sampler2D tex1;

float LinearizeDepth(float depth) 
{
    // преобразуем обратно в NDC
    float z = depth * 2.0 - 1.0; 
    return (2.0 * 0.1f * 100.0f) / (100.0f + 0.1f - z * (100.0f - 0.1f));	
}

void main()
{    
	float depth = LinearizeDepth(gl_FragCoord.z) / 100.0f;
	vec3 color0 = texture(tex0, uv).rgb;
	vec3 color1 = texture(tex1, uv).rgb;
	vec3 final = mix(color0, color1, 0.5);
    FragColor = vec4(final, 1.0);
    //FragColor = vec4(vec3(depth), 1.0);;
}