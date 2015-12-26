#include <direct.h>
#include "scene.h"
#include "view.h"
#include "light.h"
#include "glut.h"
#include "ShaderLoader.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


view *globalview;
lightsrc *globalight;
scene *globalscene;
GLhandleARB	MyShader;
int windowSize[2];
float rot_y = 0.0f;
float rot_x = 0.0f;
GLint tangent_loc, bitangent_loc;

void light();
void display();
void reshape(GLsizei , GLsizei );
void obj_display(mesh *object);
void LoadShaders();
void keyboard(unsigned char , int, int);
void computeTangenBasis();

int main(int argc, char** argv)
{
	globalview = new view("Scene3.view");
	globalight = new lightsrc("Scene3.light");

	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("HW1-2 Park");
	glewExperimental=FALSE;
	glewInit();

	globalscene = new scene("Scene3.scene");

	LoadShaders();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	delete(globalview);
	delete(globalight);

}

void light()
{
	glShadeModel(GL_SMOOTH);
	// z buffer enable
	glEnable(GL_DEPTH_TEST);
	// enable lighting
	glEnable(GL_LIGHTING);

	for (size_t i = 0; i < globalight->lTotal; i++) {
		// set light property
		glEnable(GL_LIGHT0 + i);
		glLightfv(GL_LIGHT0 + i, GL_POSITION, globalight->lightList[i].light_position);
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, globalight->lightList[i].light_diffuse);
		glLightfv(GL_LIGHT0 + i, GL_SPECULAR, globalight->lightList[i].light_specular);
		glLightfv(GL_LIGHT0 + i, GL_AMBIENT, globalight->lightList[i].light_ambient);
	}
}


void obj_display() {

	// 只有一个物体
	mesh *object = globalscene->mList[0].obejct;
	//bind texture 0
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, globalscene->mList[0].ambTextureId);
	GLint location0 = glGetUniformLocation(MyShader, "colorTexture");
	if(location0 == -1)
		printf("Cant find texture name: colorTexture\n");
	else
		glUniform1i(location0, 0);
	////bind texture 1
	//glActiveTexture( GL_TEXTURE1 );
	//glBindTexture(GL_TEXTURE_2D, globalscene->mList[0].difTextureId);
	//GLint location1 = glGetUniformLocation(MyShader, "diffuseTex");
	//if(location1 == -1)
	//	printf("Cant find texture name: diffuseTex\n");
	//else
	//	glUniform1i(location1, 0);
	////bind texture 2
	//glActiveTexture( GL_TEXTURE2 );
	//glBindTexture(GL_TEXTURE_2D, globalscene->mList[0].spcTextureId);
	//GLint location2 = glGetUniformLocation(MyShader, "specularTex");
	//if(location2 == -1)
	//	printf("Cant find texture name: specularTex\n");
	//else
	//	glUniform1i(location2, 0);

	// 确定顶点属性的 location，算完切向量后，再设置属性值
	tangent_loc = glGetAttribLocation(MyShader, "tangent");
	bitangent_loc = glGetAttribLocation(MyShader, "bitangent");

	int lastMaterial = -1;
	for(size_t i=0;i < object->fTotal;++i)
	{
		// set material property if this face used different material
		if(lastMaterial != object->faceList[i].m)
		{
			lastMaterial = (int)object->faceList[i].m;
			glMaterialfv(GL_FRONT, GL_AMBIENT  , object->mList[lastMaterial].Ka);
			glMaterialfv(GL_FRONT, GL_DIFFUSE  , object->mList[lastMaterial].Kd);
			glMaterialfv(GL_FRONT, GL_SPECULAR , object->mList[lastMaterial].Ks);
			glMaterialfv(GL_FRONT, GL_SHININESS, &object->mList[lastMaterial].Ns);

			//you can obtain the texture name by object->mList[lastMaterial].map_Kd
			//load them once in the main function before mainloop
			//bind them in display function here
		}

		// 取得三个顶点 p0 p1 p2 坐标
		float *vertex0 = object->vList[object->faceList[i][0].v].ptr;
		float *vertex1 = object->vList[object->faceList[i][1].v].ptr;
		float *vertex2 = object->vList[object->faceList[i][2].v].ptr;
		glm::vec3 p0(vertex0[0], vertex0[1], vertex0[2]);
		glm::vec3 p1(vertex1[0], vertex1[1], vertex1[2]);
		glm::vec3 p2(vertex2[0], vertex2[1], vertex2[2]);
		// 取得贴图三点 对应贴图坐标
		float *texture0 = object->tList[object->faceList[i][0].t].ptr;
		float *texture1 = object->tList[object->faceList[i][1].t].ptr;
		float *texture2 = object->tList[object->faceList[i][2].t].ptr;
		glm::vec2 UV0(texture0[0], texture0[1]);
		glm::vec2 UV1(texture1[0], texture1[1]);
		glm::vec2 UV2(texture2[0], texture2[1]);
		// 得到两边
		glm::vec3 Edge1 = p1 - p0;
		glm::vec3 Edge2 = p2 - p0;
		glm::vec2 Edge1uv = UV1 - UV0;
		glm::vec2 Edge2uv = UV2 - UV0;
		// 计算切向量，副切向量
		glm::vec3 tangent, bitangent;
		float cp = Edge1uv.x * Edge1uv.y - Edge1uv.x * Edge2uv.y;
		if(cp != 0.0f) {
			float mul = 1.0f /cp;
			tangent = (Edge1 * Edge2uv.y + Edge2 * -Edge1uv.y) * mul;
			bitangent = (Edge1 * -Edge2uv.x + Edge2 * Edge1uv.x) * mul;
		}
		// specify the value of a generic vertex attribute 设置顶点属性
		glVertexAttrib3f(tangent_loc, tangent.x, tangent.y, tangent.z);
		glVertexAttrib3f(bitangent_loc, bitangent.x, bitangent.y, bitangent.z);

		glBegin(GL_TRIANGLES);
		for (size_t j=0;j<3;++j)
		{
			//textex corrd. 
			glMultiTexCoord2fv(GL_TEXTURE0, object->tList[object->faceList[i][j].t].ptr);
			//glMultiTexCoord2fv(GL_TEXTURE1, object->tList[object->faceList[i][j].t].ptr);
			//glMultiTexCoord2fv(GL_TEXTURE2, object->tList[object->faceList[i][j].t].ptr);
			glNormal3fv(object->nList[object->faceList[i][j].n].ptr);
			glVertex3fv(object->vList[object->faceList[i][j].v].ptr);	
		}
		glEnd();
	}
}

void reshape(GLsizei w, GLsizei h)
{
	windowSize[0] = w;
	windowSize[1] = h;
}


void display() {
	// clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// 背景color
	glClearDepth(1.0f);										// Depth Buffer (深度z buffer) Setup
	glEnable(GL_DEPTH_TEST);								// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);									// The Type Of Depth Test To Do
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// 双缓冲区，z buffer

	// viewport transformation
	glViewport(globalview->viewport[0], globalview->viewport[1], globalview->viewport[2], globalview->viewport[3]);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(globalview->fovy, (GLfloat)windowSize[0]/(GLfloat)windowSize[1], globalview->dnear, globalview->dfar);
	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	globalview->eye[0], globalview->eye[1], globalview->eye[2],		// eye
		globalview->vat[0], globalview->vat[1], globalview->vat[2],     // center
		globalview->vup[0], globalview->vup[1], globalview->vup[2]);    // up

	// 设置灯光效果
	light();

	// 加载 MyShader 句柄
	glUseProgram(MyShader);

	for (int i = 0; i < globalscene->mTotla; i++) {
		glPushMatrix();
		/* 改变作图原点 */
		glTranslatef(globalscene->mList[i].translate[0], globalscene->mList[i].translate[1], globalscene->mList[i].translate[2]);
		/* 旋转变换 */
		glRotated(rot_y, 0, 1, 0);
		glRotated(rot_x, 1, 0, 0);
		glRotated(globalscene->mList[i].rotate[0], globalscene->mList[i].rotate[1], 
			globalscene->mList[i].rotate[2], globalscene->mList[i].rotate[3]);
		/* 缩放 */
		glScaled(globalscene->mList[i].scale[0], globalscene->mList[i].scale[1], globalscene->mList[i].scale[2]);
		/* 贴图方式 */
		obj_display();

		glPopMatrix();
		glFlush();
	}

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		/* ad 表示左右旋转物体， ws 表示上下旋转物体 */
		/* hj 表示左右移动光源， kl 表示上下移动光源 */
	case 'a':
		rot_y -= 60;
		break;
	case 'd':
		rot_y += 60;
		break;
	case 'w':
		rot_x -= 60;
		break;
	case 's':
		rot_x  += 60;
		break;
	case 'h':
		globalight->lightList[0].light_position[0] -= 5;
		break;
	case 'j':
		globalight->lightList[0].light_position[0] += 5;
		break;
	case 'k':
		globalight->lightList[0].light_position[1] -= 5;
		break;
	case 'l':
		globalight->lightList[0].light_position[1] += 5;
		break;
	case 27:     exit(0);
	}
	glutPostRedisplay();
}

void LoadShaders()
{
	MyShader = glCreateProgram();
	if(MyShader != 0)
	{
		ShaderLoad(MyShader, "../UseTexture.vs", GL_VERTEX_SHADER);
		ShaderLoad(MyShader, "../UseTexture.frag", GL_FRAGMENT_SHADER);
	}
}

void computeTangenBasis()
{
	mesh *object = globalscene->mList[0].obejct;
}
