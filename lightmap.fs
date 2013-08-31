
varying vec2 uv;

uniform sampler2D textureId;

void main()
{
	//gl_FragColor.rgb = texture2D(textureId, uv, -2.0);
	gl_FragColor = texture2D(textureId, uv, -2.0);
}
