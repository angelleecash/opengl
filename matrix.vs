
attribute vec3 modelPosition;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(modelPosition, 1.0);
}
