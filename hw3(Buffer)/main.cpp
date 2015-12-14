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

int main(int argc, char** argv)
{
	globalview = new view("Scene1.view");
	globalight = new lightsrc("Scene1.light");

	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("HW3 Buffer");
	globalscene = new scene("Scene1.scene");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
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
			//textex corrd. 
			//glTexCoord2fv(object->tList[object->faceList[i][j].t].ptr);
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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// ����color
	glClearDepth(1.0f);										// Depth Buffer (���z buffer) Setup
	glEnable(GL_DEPTH_TEST);								// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);									// The Type Of Depth Test To Do
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// ˫��������z buffer

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

	// ���õƹ�Ч��
	light();

	for (int i = 0; i < globalscene->mTotla; i++) {
		glPushMatrix();
		/* �ı���ͼԭ�� */
		glTranslatef(globalscene->mList[i].translate[0], globalscene->mList[i].translate[1], globalscene->mList[i].translate[2]);
		/* ��ת�任 */
		glRotated(globalscene->mList[i].rotate[0], globalscene->mList[i].rotate[1], 
			globalscene->mList[i].rotate[2], globalscene->mList[i].rotate[3]);
		// ����
		glScaled(globalscene->mList[i].scale[0], globalscene->mList[i].scale[1], globalscene->mList[i].scale[2]);
		// ��������
		obj_display(globalscene->mList[i].obejct);

		glPopMatrix();
		glFlush();
	}

	glutSwapBuffers();
}