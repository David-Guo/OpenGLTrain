#include <direct.h>
#include "scene.h"
#include "view.h"
#include "light.h"
#include "glut.h"

view *globalview;
lightsrc *globalight;
scene *globalscene;

void light();
void display();
void reshape(GLsizei , GLsizei );
void obj_display(mesh *object);
int windowSize[2];
float rot_y = 0.0f;
float rot_x = 0.0f;
void keyboard(unsigned char , int, int);

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
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, globalscene->mList[0].ambTextureId);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
	//bind texture 1
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, globalscene->mList[0].difTextureId);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
	//bind texture 2
	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, globalscene->mList[0].spcTextureId);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

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

		glBegin(GL_TRIANGLES);
		for (size_t j=0;j<3;++j)
		{
			//textex corrd. 
			glMultiTexCoord2fv(GL_TEXTURE0, object->tList[object->faceList[i][j].t].ptr);
			glMultiTexCoord2fv(GL_TEXTURE1, object->tList[object->faceList[i][j].t].ptr);
			glMultiTexCoord2fv(GL_TEXTURE2, object->tList[object->faceList[i][j].t].ptr);
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