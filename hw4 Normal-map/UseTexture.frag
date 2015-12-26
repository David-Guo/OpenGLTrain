uniform sampler2D colorTexture;
//uniform sampler2D diffuseTex;
//uniform sampler2D specularTex;

void main (void)
{
	gl_FragColor = texture2D(colorTexture, gl_TexCoord[0].xy).rgba;	
}