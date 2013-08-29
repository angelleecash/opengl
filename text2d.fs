varying vec2 outputUv;

uniform sampler2D textureImage; 

void main()
{
	gl_FragColor = texture2D(textureImage, outputUv);
//	gl_FragColor.a = 1.0;
}
