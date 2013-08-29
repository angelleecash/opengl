attribute vec2 position;
attribute vec2 uv;

varying vec2 outputUv;

void main()
{
	vec2 vertexPosition = position - vec2(400, 300);
	vertexPosition /= vec2(400, 300);

	gl_Position = vec4(vertexPosition, 0, 1);

	outputUv = uv;
}
