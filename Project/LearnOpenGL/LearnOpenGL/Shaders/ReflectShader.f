#version 330 core
out vec4 FragColor;

in vec2 uv;
in vec3 aNormal;
in vec3 position;

uniform sampler2D tex;
uniform samplerCube cubeMap;
uniform sampler2D tex1;
uniform vec3 cameraPos;
float k = 0.658;
void main()
{    
	vec3 v = normalize(position - cameraPos);
	vec3 r = reflect(v, normalize(aNormal));
	//vec3 r = refract(v, normalize(aNormal), k);
	vec3 cube = texture(cubeMap, r).rgb;
	vec3 color = texture(tex, uv).rgb;
	vec3 final = mix(vec3(0.5), cube, color.rgb);
    FragColor = vec4(final, 1.0);
    //FragColor = vec4(texture(tex, uv).bbb, 1.0);
}