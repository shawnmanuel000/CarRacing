#version 440

struct Material
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	int Ns;				
	float d;				
	int illum;				
	sampler2D diffuseTex;
	sampler2D specularTex;
};

struct PointLight
{
	vec3 position;
	float intensity;
	float constant;
	float linear;
	float quadratic;
};

in vec3 vs_position;
in vec2 vs_texcoord;
in vec3 vs_normal;
in vec3 vs_color;

out vec4 fs_color;

//Uniforms
uniform Material material;
uniform PointLight pointLight;
uniform vec3 lightPos0;
uniform vec3 cameraPos;

//Functions
vec3 calculateAmbient(Material material)
{
	float weight = 0.1;
	return vec3(weight*material.Ka.r, weight*material.Ka.g, weight*material.Ka.b);
}

vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0)
{
	vec3 posToLightDirVec = normalize(lightPos0 - vs_position);
	float Kd = clamp(dot(posToLightDirVec, normalize(vs_normal)), 0.5, 1.0);
	vec3 diffuseFinal = material.Kd * Kd;
	return diffuseFinal;
}

vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0, vec3 cameraPos)
{
	vec3 lightToPosDirVec = normalize(vs_position - lightPos0);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(cameraPos - vs_position);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), material.Ns);
	//vec3 specularFinal = material.Ks * specularConstant * texture(material.specularTex, vs_texcoord).rgb;
	vec3 specularFinal = material.Ks * specularConstant;
	return specularFinal;
}

void main()
{
	vec3 ambientFinal = calculateAmbient(material);
	vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, pointLight.position);
	vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, pointLight.position, cameraPos);
	vec4 tex = texture(material.diffuseTex, vs_texcoord);
	// fs_color = !hastexture ? vec4(vs_color, 1.f) : texture(material.diffuseTex, vs_texcoord) * (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
	// fs_color = vec4(vs_color, 1.f) + Kd + vec4(material.Kd, 1.f);// * (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f));
	// fs_color = texture(texture0, vs_texcoord) * texture(texture1, vs_texcoord) * vec4(vs_color, 1.f);	
	// float distance = length(pointLight.position - vs_position);
	// float attenuation = pointLight.constant / (1.f + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
	// ambientFinal *= attenuation;
	// diffuseFinal *= attenuation;
	// specularFinal *= attenuation;
	// fs_color = texture(material.diffuseTex, vs_texcoord) * (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
	fs_color = tex + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f);
	fs_color.a = material.d;
}
