uniform sampler2D colorTexture;

void main (void)
{
	gl_FragColor = texture2D(colorTexture, gl_TexCoord[0].xy).rgba;	
}