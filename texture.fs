//varying vec3 fragmentColor;

varying vec2 uv;
uniform sampler2D textureId;

void main()
{
	//gl_FragColor = vec4(fragmentColor, 1);
	gl_FragColor = texture2D(textureId, uv);
}
