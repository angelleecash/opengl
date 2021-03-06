#version 120

varying vec2 uv;
varying vec3 positionInWorldSpace;
varying vec3 normalInCameraSpace;
varying vec3 eyeDirectionInCameraSpace;
varying vec3 lightDirectionInCameraSpace;
varying vec4 shadowPosition;

uniform sampler2D textureId;
uniform sampler2DShadow shadowMap;
uniform vec3 lightPositionInWorld;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

float random(vec3 seed, int i)
{
	vec4 seed4 = vec4(seed, i);
	float dotProduct = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
	return fract(sin(dotProduct) * 43758.5453);
}

void main()
{
	vec3 lightColor = vec3(1 ,1, 1);
	float lightPower = 1.0;

	vec3 materialDiffuseColor = texture2D(textureId, uv).rgb;
	vec3 materialAmbientColor = vec3(0.1, 0.1, 0.1) * materialDiffuseColor;
	vec3 materialSpecularColor = vec3(0.3, 0.3, 0.3);

	float distanceToLight = length(positionInWorldSpace - lightPositionInWorld);

	vec3 vertexNormal = normalize(normalInCameraSpace);
	vec3 lightNormal = normalize(lightDirectionInCameraSpace);
	float cosTheta = clamp(dot(vertexNormal, lightNormal), 0.0, 1.0);
	
	vec3 eyeDirectionInCameraSpaceNormal = normalize(eyeDirectionInCameraSpace);
	vec3 reflectResult = reflect(-lightNormal, vertexNormal);
	float cosAlpha = clamp(dot(eyeDirectionInCameraSpaceNormal, reflectResult), 0.0, 1.0);

	float visibility = 1.0;
	float bias = 0.005;

	for(int i=0; i < 4; i++)
	{
		visibility -= 0.2*(1.0 - shadow2D(shadowMap, vec3(shadowPosition.xy, (shadowPosition.z-bias)/shadowPosition.w)).r);
	}

	float distanceToLightSquare = distanceToLight*distanceToLight;

	vec3 diffuseComponent = visibility * materialDiffuseColor * lightColor * lightPower * cosTheta / distanceToLightSquare;

	vec3 specularComponent = visibility * materialSpecularColor * lightColor * lightPower * pow(cosAlpha, 5.0)/distanceToLightSquare;


	gl_FragColor.rgb = materialAmbientColor + diffuseComponent + specularComponent;
}
