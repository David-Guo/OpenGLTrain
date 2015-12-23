#include "ShaderLoader.h"
#include <stdio.h>
#include <stdlib.h>
#include "glew.h"
#include "glut.h"


bool ShaderLoad(GLhandleARB programId, char* shaderSrc, GLenum shaderType)
{
	FILE *fp;
	GLhandleARB h_shader;
	GLcharARB *shader_string;
	GLint str_length, maxLength;
	GLint isCompiled = GL_FALSE, isLinked = GL_FALSE;
	GLcharARB *pInfoLog;

	// open the file of shader source code
	if((fp=fopen(shaderSrc, "r")) == NULL)
	{
		fprintf(stderr, "Error : Failed to read the OpenGL shader source \"%s\".\n", shaderSrc);
		return false;
	}

	// allocate memory for program string and load it.
	shader_string = (GLcharARB*) malloc(sizeof(GLcharARB) * 65536);
	str_length = (GLint) fread(shader_string, 1, 65536, fp);
	fclose(fp);

	// Create and load shader string.
	h_shader = glCreateShader(shaderType);
	if(h_shader == 0)
	{
		fprintf(stderr, "Error : Failed to create OpenGL shader object \"%s\".\n", shaderSrc);
		return false;
	}
	glShaderSource(h_shader, 1, (const GLcharARB**)&shader_string, &str_length);
	free(shader_string);

	// Compile the vertex shader, print out the compiler log message.
	glCompileShader(h_shader);

	// get compile state information
	glGetObjectParameterivARB(h_shader, GL_OBJECT_COMPILE_STATUS_ARB, &isCompiled);

	if(!isCompiled)
	{
		fprintf(stderr, "Error : Failed to compile OpenGL shader source \"%s\".\n", shaderSrc);
		glGetObjectParameterivARB(h_shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);
		pInfoLog = (GLcharARB *) malloc(maxLength * sizeof(GLcharARB));
		glGetInfoLogARB(h_shader, maxLength, &str_length, pInfoLog);
		fprintf(stderr, "%s\n", pInfoLog);
		free(pInfoLog);
		return false;
	}
	glAttachShader(programId, h_shader);

	// delete the shader object, since we have attached it with the program object.
	glDeleteShader(h_shader);

	// Link the program and print out the linker log message
	glLinkProgram(programId);
	glGetObjectParameterivARB(programId, GL_OBJECT_LINK_STATUS_ARB, &isLinked);

	if(!isLinked)
	{
		fprintf(stderr, "Error : Failed to link OpenGL shader \"%s\".\n", shaderSrc);
		glGetObjectParameterivARB(programId, GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);
		pInfoLog = (GLcharARB *) malloc(maxLength * sizeof(GLcharARB));
		glGetInfoLogARB(programId, maxLength, &str_length, pInfoLog);
		fprintf(stderr, "%s\n", pInfoLog);
		free(pInfoLog);
		return false;
	}

	return true;
}
