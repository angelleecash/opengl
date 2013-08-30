#version 120

attribute vec3 modelPosition;
//attribute vec3 modelColor;
attribute vec2 texturePosition;
attribute vec3 normal;
attribute vec3 vertexTangent;
attribute vec3 vertexBitangent;

//varying vec3 fragmentColor;
varying vec2 uv;
varying vec3 positionInWorldSpace;
varying vec3 normalInCameraSpace;
varying vec3 eyeDirectionInCameraSpace;
varying vec3 lightDirectionInCameraSpace;


varying vec3 eyeDirectionInTangentSpace;
varying vec3 lightDirectionInTangentSpace;

uniform mat4 mvp;
uniform mat4 camera;
uniform mat4 world;
uniform vec3 lightPosition;
uniform mat3 worldCamera3x3;

void main()
{
	gl_Position = mvp * vec4(modelPosition, 1.0);
	positionInWorldSpace = (world*vec4(modelPosition,1)).xyz;
	vec3 positionInCameraSpace = (camera*world*vec4(modelPosition,1)).xyz;
	eyeDirectionInCameraSpace = vec3(0, 0, 0) - positionInCameraSpace;

	vec3 lightPositionInCameraSpace = (camera*vec4(lightPosition,1)).xyz;
	lightDirectionInCameraSpace = lightPositionInCameraSpace + eyeDirectionInCameraSpace;

	normalInCameraSpace = (camera*world*vec4(normal, 0)).xyz;

	//fragmentColor = modelColor;
	uv = texturePosition; 

	vec3 vertexTangentInCameraSpace = worldCamera3x3 * vertexTangent;
	vec3 vertexBitangentInCameraSpace = worldCamera3x3 * vertexBitangent;
	vec3 vertexNormalInCameraSpace = worldCamera3x3 * normal;

	mat3 TBN = transpose(mat3(vertexTangentInCameraSpace, vertexBitangentInCameraSpace, vertexNormalInCameraSpace));


	lightDirectionInTangentSpace = TBN * lightDirectionInCameraSpace;
	eyeDirectionInTangentSpace = TBN * eyeDirectionInCameraSpace;
}
