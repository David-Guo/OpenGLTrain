attribute vec3 tangent;
attribute vec3 bitangent;

varying vec3 normal, lightDir, eyeDir;
varying vec3 T_var, B_var;

void main()
{	
	// 转换到视角坐标，下面所有计算在视角坐标系中完成
	// 法向量
	normal = gl_NormalMatrix * gl_Normal;

	// 光源到顶点向量
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);

	// 视点到顶点向量
	eyeDir = -vVertex;

	// 切向量，传递到 fragment shader 中去
	T_var = gl_NormalMatrix * tangent;
	B_var = gl_NormalMatrix * bitangent;

	// 取得贴图坐标
	// build-in 输入 gl_MultiTexCoord0~n， build-in 输出 gl_TexCorrd[0~n]
	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;
	gl_TexCoord[1].xy = gl_MultiTexCoord1.xy;
	gl_TexCoord[2].xy = gl_MultiTexCoord2.xy;

	// 得到顶点属性后，还需要拿到顶点坐标
	gl_Position = ftransform();	
}