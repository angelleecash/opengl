
attribute vec3 vertexPosition;

varying vec2 uv;

void main()
{
	gl_Position = vec4(vertexPosition, 1);
	uv = (vertexPosition.xy+vec2(1,1)) / 2.0;
}
