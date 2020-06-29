#version 330 core
out vec4 FragColor;
in vec2 uv;
in vec4 worldPos;
in vec4 n;
in vec4 tangent;
in vec4 biTangent;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D glossMap;
uniform vec3 ligthPos;
uniform vec3 viewPos;

uniform int f;

void main()
{    

	mat3 TBN = mat3(normalize(tangent.xyz), normalize(biTangent.xyz), normalize(n.xyz));
	vec3 lightDir = normalize(ligthPos - worldPos.xyz);
	
	vec3 tn = texture(normalMap, uv).rgb;
	tn = tn * 2 - 1;
	//TBN  = transpose(TBN);
	tn = TBN * tn;
	float diff = max(dot(tn.xyz, lightDir), 0.0);
	FragColor = vec4(vec3(diff), 1.0);
}