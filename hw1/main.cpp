#include "mesh.h"
#include "glut.h"
#include "view.h"
#include "light.h"
#include "scene.h"

mesh *objects[3];
view *globalview;
lightsrc *globalight;
scene *globalscene;
static GLfloat updown = 0.0f;
static GLfloat movelr = 0.0f;
static GLfloat inout  = 0.0f;
static GLfloat ob_updown = 0.0f;
static GLfloat ob_movelr = 0.0f;
float mouseX;
float mouseY;
float objectMove[3][2] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

int windowSize[2];

void light();
void display();
void reshape(GLsizei , GLsizei );
void obj_display(mesh *object);
void keyboard(unsigned char , int, int);
void mouseMotion(int x, int y);
void mouse(int button, int state, int x, int y);

int main(int argc, char** argv)
{
	objects[0] = new mesh("box.obj");
	objects[1] = new mesh("venus.obj");
	objects[2] = new mesh("bunny.obj");
	globalview = new view("TestScene1/view.view");
	globalight = new lightsrc("TestScene1/light.light");
	globalscene = new scene("TestScene1/scene.scene");



	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("HW1 TestScene1");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutMainLoop();

	delete(globalscene);
	delete(objects[0]);
	delete(objects[1]);
	delete(objects[2]);
	delete(globalview);
	delete(globalight);
	return 0;
}

void light()
{
	glShadeModel(GL_SMOOTH);

	// z buffer enable
	glEnable(GL_DEPTH_TEST);

	// enable lighting
	glEnable(GL_LIGHTING);
	
	
	for (size_t i = 0; i < 2; i++) {
		// set light property
		glEnable(GL_LIGHT0 + i);
		glLightfv(GL_LIGHT0 + i, GL_POSITION, globalight->lightList[i].light_position);
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, globalight->lightList[i].light_diffuse);
		glLightfv(GL_LIGHT0 + i, GL_SPECULAR, globalight->lightList[i].light_specular);
		glLightfv(GL_LIGHT0 + i, GL_AMBIENT, globalight->lightList[i].light_ambient);
	}
}

void display()
{
	// clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// 背景color
	glClearDepth(1.0f);										// Depth Buffer (深度z buffer) Setup
	glEnable(GL_DEPTH_TEST);								// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);									// The Type Of Depth Test To Do
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// 双缓冲区，z buffer

	// viewport transformation
	// glViewport(0, 0, 800, 800);
	glViewport(globalview->viewport[0], globalview->viewport[1], globalview->viewport[2], globalview->viewport[3]);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(globalview->fovy, (GLfloat)windowSize[0]/(GLfloat)windowSize[1], globalview->dnear, globalview->dfar);
	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	globalview->eye[0] + movelr, globalview->eye[1] + updown, globalview->eye[2] + inout,		// eye
				globalview->vat[0], globalview->vat[1], globalview->vat[2],     // center
				globalview->vup[0], globalview->vup[1], globalview->vup[0]);    // up

	// 设置灯光效果
	light();

	for (int i = 0; i < 3; i++) {
		glPushMatrix();
			/* 旋转变换 */
			glRotated(globalscene->mList[i].rotate[0], globalscene->mList[i].rotate[1], 
				globalscene->mList[i].rotate[2], globalscene->mList[i].rotate[3]);
			/* 改变作图原点 */
			glTranslatef(globalscene->mList[i].translate[0] - objectMove[i][0], 
				globalscene->mList[i].translate[1] + objectMove[i][1], 
				globalscene->mList[i].translate[2]);
			/* 画出物体 */
			obj_display(objects[i]);
		glPopMatrix();
	}

	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	windowSize[0] = w;
	windowSize[1] = h;
}

void obj_display(mesh *object) {

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
			//textex corrd. object->tList[object->faceList[i][j].t].ptr
			glNormal3fv(object->nList[object->faceList[i][j].n].ptr);
			glVertex3fv(object->vList[object->faceList[i][j].v].ptr);	
		}
		glEnd();
	}
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	/* ad 表示左右移动， ws 表示上下移动相机 */
	case 'a':
		movelr -= 30;
		glutPostRedisplay();
		break;
	case 'd':
		movelr += 30;
		glutPostRedisplay();
		break;
/*	case 'w':
		updown += 20;
		glutPostRedisplay();
		break;
	case 's':
		updown -= 20;
		glutPostRedisplay();
		break;				*/
	case 'w':
		inout  -= 100;
		glutPostRedisplay();
		break;
	case 's':
		inout  += 100;
		glutPostRedisplay();
		break;
	case '1':
		objectMove[0][0] = ob_movelr;
		objectMove[0][1] = ob_updown;
		glutPostRedisplay();
		break;
	case '2':
		objectMove[1][0] = ob_movelr;
		objectMove[1][1] = ob_updown;
		glutPositionWindow;
		break;
	case '3':
		objectMove[2][0] = ob_movelr;
		objectMove[2][1] = ob_updown;
		glutPostRedisplay();
		break;
	case 27:     exit(0);
	}
}

void mouse(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;
}

void mouseMotion(int x, int y)
{
	ob_movelr = mouseX - x;
	ob_updown = mouseY - y;
	glutPostRedisplay();
}
