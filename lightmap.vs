
attribute vec3 modelPosition;
attribute vec2 texturePosition;

varying vec2 uv;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(modelPosition, 1.0);
	uv = texturePosition; 
}
