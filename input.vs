
attribute vec3 modelPosition;
//attribute vec3 modelColor;
attribute vec2 texturePosition;

//varying vec3 fragmentColor;
varying vec2 uv;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(modelPosition, 1.0);
	//fragmentColor = modelColor;
	uv = texturePosition; 
}
