#version 330 core

out vec4 FragColor;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec4 FragPosLightSpace;
} fs_in;

// uniform sampler2D duffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// [-1,1] transform to [0, 1]
	projCoords = projCoords * 0.5 + 0.5;
	// get closest depth from depthmap
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	float shadow = (currentDepth > closestDepth ? 1.0 : 0.0);

	return shadow;
}

void main()
{
	float shiness = 128.0;

	// vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
	vec3 color = vec3(1.0, 1.0, 1.0);
	vec3 normal = normalize(fs_in.Normal);

	// ambient
	vec3 ambient = 0.3 * color;

	// diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	float decay = 0.1 * distance(lightPos, fs_in.FragPos);
	vec3 diffuse = decay * diff * lightColor; // TODO: consider decay

	// specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), shiness);
    vec3 specular = spec * lightColor;  

	float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
	// float shadow = 0.0;
	vec3 lighting = (ambient + (1.0 - shadow)*(diffuse + specular)) * color;

	FragColor = vec4(lighting, 1.0);
}