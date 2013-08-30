#version 120
//varying vec3 fragmentColor;

varying vec2 uv;
varying vec3 positionInWorldSpace;
varying vec3 normalInCameraSpace;
varying vec3 eyeDirectionInCameraSpace;
varying vec3 lightDirectionInCameraSpace;

varying vec3 eyeDirectionInTangentSpace;
varying vec3 lightDirectionInTangentSpace;

uniform sampler2D diffuseTextureId;
uniform sampler2D normalTextureId;
uniform sampler2D specularTextureId;

uniform vec3 lightPosition;
uniform mat3 worldCamera3x3;

void main()
{
	vec3 lightColor = vec3(1 ,1, 1);
	float lightPower = 50.0;

	vec3 materialDiffuseColor = texture2D(diffuseTextureId, uv).rgb;
	vec3 materialAmbientColor = vec3(0.1, 0.1, 0.1) * materialDiffuseColor;
	vec3 materialSpecularColor = texture2D(specularTextureId, uv).rgb * 0.3;
	vec3 textureNormal = normalize(texture2D(normalTextureId, vec2(uv.x, -uv.y)).rgb * 2.0 - 1.0);

	float distanceToLight = length(positionInWorldSpace - lightPosition);
	//vec3 vertexNormal = normalize(normalInCameraSpace);
	//vec3 lightNormal = normalize(lightDirectionInCameraSpace);

	vec3 vertexNormal = textureNormal;
	vec3 lightNormal = normalize(lightDirectionInTangentSpace);

	float cosTheta = clamp(dot(vertexNormal, lightNormal), 0.0, 1.0);
	
	vec3 eyeDirectionInCameraSpaceNormal = normalize(eyeDirectionInTangentSpace);
	vec3 reflectResult = reflect(-lightNormal, vertexNormal);

	float cosAlpha = clamp(dot(eyeDirectionInCameraSpaceNormal, reflectResult), 0.0, 1.0);


	//gl_FragColor = vec4(fragmentColor, 1);
	//igl_FragColor = texture2D(textureId, uv);

	float distanceToLightSquare = distanceToLight*distanceToLight;

	vec3 diffuseComponent = materialDiffuseColor * lightColor * lightPower * cosTheta / distanceToLightSquare;

	vec3 specularComponent = materialSpecularColor * lightColor * lightPower * pow(cosAlpha, 5.0)/distanceToLightSquare;


	gl_FragColor.rgb = materialAmbientColor + diffuseComponent + specularComponent;
	//gl_FragColor.a = 1.0;
	gl_FragColor.a = 0.5;
}
