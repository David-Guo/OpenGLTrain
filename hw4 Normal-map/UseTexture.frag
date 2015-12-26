uniform sampler2D colorTexture;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;

varying vec3 normal, lightDir, eyeDir;
varying vec3 T_var, B_var;

void main (void)
{
    
    vec4 final_color = 
	(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
	(gl_LightSource[0].ambient * gl_FrontMaterial.ambient);

    vec3 T = normalize(T_var);
	vec3 B = normalize(B_var);
	vec3 normN = normalize(normal);

    // 法线贴图
    vec4 pC = texture2D(colorTexture, gl_TexCoord[0].xy).rgba;
	pC.r -= 0.5;
	pC.r *= 2.0;
	pC.g -= 0.5;
	pC.g *= 2.0;
	pC.b -= 0.5;
	pC.b *= 2.0;

    // 计算新的法线
    vec3 new_normal = pC.r * T + pC.g * B + pC.b * normN;

    vec3 N = normalize(new_normal);
	vec3 L = normalize(lightDir);

	// 从贴图中获得 diffuse
	vec4 diffuse = texture2D(diffuseTex, gl_TexCoord[1].xy).rgba;
	vec4 specular = texture2D(specularTex, gl_TexCoord[2].xy).rgba;

    // Phong Shading 模型事务
    // 最终光照 = diffuse + specular
    float lambertTerm = dot(N,L);
    if(lambertTerm > 0.0)
	{
        //final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;

		final_color += diffuse * lambertTerm;

        vec3 E = normalize(eyeDir);
        vec3 R = reflect(-L, N);
        float specularTerm = pow(max(dot(R, E), 0.0), gl_FrontMaterial.shininess );
		
		final_color += specular * specularTerm;

		//final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;
		
    }

	gl_FragColor = final_color;
}