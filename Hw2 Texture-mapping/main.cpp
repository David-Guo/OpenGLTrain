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
void setTexture(int i);

int main(int argc, char** argv)
{
	char* buffer;
	// Get the current working directory: 
	if( (buffer = _getcwd( NULL, 0 )) == NULL )
		perror( "_getcwd error" );
	else
	{
		printf( "%s \nLength: %d\n", buffer, strlen(buffer));
		free(buffer);
	}

	globalview = new view("park.view");
	globalight = new lightsrc("park.light");
	/* Please set the working dirctory as ./Chess  */
	//globalview = new view("Chess.view");
	//globalight = new lightsrc("Chess.light");

	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("HW1-2 Park");
	glewExperimental=FALSE;
	glewInit();
	globalscene = new scene("park.scene");
	//globalscene = new scene("Chess.scene");
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
			//if (globalscene->mList[i].texWay != 3)
			glTexCoord2fv(object->tList[object->faceList[i][j].t].ptr);
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
		glRotated(globalscene->mList[i].rotate[0], globalscene->mList[i].rotate[1], 
			globalscene->mList[i].rotate[2], globalscene->mList[i].rotate[3]);
		/* 缩放 */
		glScaled(globalscene->mList[i].scale[0], globalscene->mList[i].scale[1], globalscene->mList[i].scale[2]);
		/* 贴图方式 */
		setTexture(i);
		//obj_display(globalscene->mList[i].obejct);

		glPopMatrix();
		glFlush();
	}

	glutSwapBuffers();
}


void setTexture(int i) {
	switch (globalscene->mList[i].texWay)
	{
	case 0:
		/* 画出物体 */
		obj_display(globalscene->mList[i].obejct);
		break;
	case 1:
		// 必须要开启alpha test 把透明的地方挖空！
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.5f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, globalscene->mList[i].texObject1);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		/* 画出物体 */
		obj_display(globalscene->mList[i].obejct);
		/* 关闭贴图 */
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
		glBindTexture(GL_TEXTURE_2D, 0);
		break;
	case 2:
		//bind texture 0
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, globalscene->mList[i].texObject1);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
		//bind texture 1
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, globalscene->mList[i].texObject2);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

		/* 画出物体 */
		// 两幅贴图的物体，贴图方式有所不同，需要另外完成，画图功能，不能直接调用 obj_display()
		{
			int lastMaterial = -1;
			for(size_t k=0;k < globalscene->mList[i].obejct->fTotal;++k)
			{
				// set material property if this face used different material
				if(lastMaterial !=globalscene->mList[i].obejct->faceList[k].m)
				{
					lastMaterial = (int)globalscene->mList[i].obejct->faceList[k].m;
					glMaterialfv(GL_FRONT, GL_AMBIENT  , globalscene->mList[i].obejct->mList[lastMaterial].Ka);
					glMaterialfv(GL_FRONT, GL_DIFFUSE  , globalscene->mList[i].obejct->mList[lastMaterial].Kd);
					glMaterialfv(GL_FRONT, GL_SPECULAR , globalscene->mList[i].obejct->mList[lastMaterial].Ks);
					glMaterialfv(GL_FRONT, GL_SHININESS, &globalscene->mList[i].obejct->mList[lastMaterial].Ns);

					//you can obtain the texture name by object->mList[lastMaterial].map_Kd
					//load them once in the main function before mainloop
					//bind them in display function here
				}

				glBegin(GL_TRIANGLES);
				for (size_t j=0;j<3;++j)
				{
					//textex corrd. 
					glMultiTexCoord2fv(GL_TEXTURE0, globalscene->mList[i].obejct->tList[globalscene->mList[i].obejct->faceList[k][j].t].ptr);
					glMultiTexCoord2fv(GL_TEXTURE1, globalscene->mList[i].obejct->tList[globalscene->mList[i].obejct->faceList[k][j].t].ptr);
					glNormal3fv(globalscene->mList[i].obejct->nList[globalscene->mList[i].obejct->faceList[k][j].n].ptr);
					glVertex3fv(globalscene->mList[i].obejct->vList[globalscene->mList[i].obejct->faceList[k][j].v].ptr);	
				}
				glEnd();
			}
		}
		//unbind texture 1
		glActiveTexture(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		//unbind texture 0
		glActiveTexture(GL_TEXTURE0);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		break;
	case 3:
		//bind texture 0
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
		glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		glEnable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, globalscene->mList[i].texObject1);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		/* 画出物体 */
		obj_display(globalscene->mList[i].obejct);
		/* 关闭贴图 */
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		glDisable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		break;
	}
}