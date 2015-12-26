attribute vec3 tangent;
attribute vec3 bitangent;

void main()
{	
	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;
	gl_Position = ftransform();	
}