#include "ShaderLoader.h"
#include "mesh.h"
#include "FreeImage.h"
#include <stdio.h>
#include <stdlib.h>
#include "glew.h"
#include "glut.h"


#define		TEX_NUM 1	  //the number of textures you use.
GLuint		texObject[TEX_NUM];	//texture object
GLhandleARB	MyShader;
mesh*		object;
int			WinW,WinH;

unsigned int LoadTexture(const char* filename);
void LoadShaders();
void Display();
void Reshape(GLsizei , GLsizei );

int main(int argc, char** argv)
{
	object = new mesh("sphere.obj");

	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Shader Use Texture");

	GLenum glew_error;
	if((glew_error = glewInit()) != GLEW_OK)return -1;

	FreeImage_Initialise();
	texObject[0] = LoadTexture("world.bmp");
	FreeImage_DeInitialise();

	LoadShaders();

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMainLoop();

	return 0;
}

unsigned int LoadTexture(const char* pszFile)
{
	FIBITMAP* pImage = FreeImage_Load(FreeImage_GetFileType(pszFile, 0), pszFile);
	FIBITMAP *p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
	int iWidth = FreeImage_GetWidth(p32BitsImage);
	int iHeight = FreeImage_GetHeight(p32BitsImage);

	unsigned int iTextureId;
	glGenTextures(1, &iTextureId);
	glBindTexture(GL_TEXTURE_2D, iTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iWidth, iHeight,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));

	FreeImage_Unload(p32BitsImage);
	FreeImage_Unload(pImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return iTextureId;
}

void LoadShaders()
{
	MyShader = glCreateProgram();
	if(MyShader != 0)
	{
		ShaderLoad(MyShader, "../UseTexture.vs", GL_VERTEX_SHADER);
		ShaderLoad(MyShader, "../UseTexture.fs", GL_FRAGMENT_SHADER);
	}
}

void Display()
{
	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)WinW/(GLfloat)WinH, 1.0, 1000.0);
	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 10.0,     // eye
		0.0, 0.0, 0.0,     // center
		0.0, 1.0, 0.0);    // up

	// clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      //清除用color
	glClearDepth(1.0f);                        // Depth Buffer (就是z buffer) Setup
	glEnable(GL_DEPTH_TEST);                   // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                    // The Type Of Depth Test To Do
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//這行把畫面清成黑色並且清除z buffer

	glUseProgram(MyShader);

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, texObject[0]);
	GLint location = glGetUniformLocation(MyShader, "colorTexture");
	if(location == -1)
		printf("Cant find texture name: colorTexture\n");
	else
		glUniform1i(location, 0);

	for (size_t i=0;i < object->fTotal;i++)
	{
		glBegin(GL_POLYGON);
		for (size_t j=0;j<3;j++)
		{
			glMultiTexCoord2fv(GL_TEXTURE0,	object->tList[object->faceList[i][j].t].ptr);
			glNormal3fv(object->nList[object->faceList[i][j].n].ptr);
			glVertex3fv(object->vList[object->faceList[i][j].v].ptr);	
		}
		glEnd();
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

void Reshape(GLsizei w, GLsizei h)
{
	WinW = w;
	WinH = h;
	// viewport transformation
	glViewport(0, 0, WinW,WinH);
}