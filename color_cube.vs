
attribute vec3 modelPosition;
attribute vec3 modelColor;

varying vec3 fragmentColor;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(modelPosition, 1.0);
	fragmentColor = modelColor;
}
