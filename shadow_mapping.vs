#version 120

attribute vec3 modelPosition;
attribute vec2 texturePosition;
attribute vec3 normal;

varying vec2 uv;
varying vec3 positionInWorldSpace;
varying vec3 normalInCameraSpace;
varying vec3 eyeDirectionInCameraSpace;
varying vec3 lightDirectionInCameraSpace;
varying vec4 shadowPosition;

uniform mat4 mvp;
uniform mat4 camera;
uniform mat4 world;
//uniform vec3 lightPosition;
uniform vec3 lightDirectionInvWorld;
uniform mat4 depthMvp;

void main()
{
	gl_Position = mvp * vec4(modelPosition, 1.0);
	shadowPosition = depthMvp * vec4(modelPosition, 1.0);

	positionInWorldSpace = (world*vec4(modelPosition,1)).xyz;

	vec3 positionInCameraSpace = (camera*world*vec4(modelPosition,1)).xyz;

	eyeDirectionInCameraSpace = vec3(0, 0, 0) - positionInCameraSpace;

	lightDirectionInCameraSpace = (camera*vec4(lightDirectionInvWorld, 0)).xyz;
	//original light calculation
	//lightDirectionInCameraSpace = lightPositionInCameraSpace + eyeDirectionInCameraSpace;

	normalInCameraSpace = (camera*world*vec4(normal, 0)).xyz;

	uv = texturePosition; 
}
