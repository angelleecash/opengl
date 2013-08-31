varying vec2 uv;

uniform sampler2D texture;
uniform float time;

void main()
{
	gl_FragColor = texture2D(texture, uv + 0.005*vec2(sin(time+800.0*uv.x), cos(time+600.0*uv.y)));
}
