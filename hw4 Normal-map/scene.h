// scene.h: interface for the scene class.
// author: 郭大为（David Guo)
// time  : 2015/10/18
// func  : read view data from file *.scene for class scene
//////////////////////////////////////////////////////////////////////

#ifndef AFX_SCENE_H_
#define AFX_SCENE_H_
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "FreeImage.h"
#include "glew.h"
#include "mesh.h"



using namespace std;


class scene {

public:
	class model{
	public:
		
		float scale[3];
		float rotate[4];
		float translate[3];

		mesh *obejct;

		GLuint ambTextureId;
		GLuint difTextureId;
		GLuint spcTextureId;

	public:
		/* model 构造函数 */
		model(const char *objName, float *s, float *r, float *t) {
			for(int i = 0; i < 3; i++) {
				scale[i] = s[i];
				rotate[i] = r[i];
				translate[i] = t[i];
			}
			rotate[3] = r[3];
			obejct = new mesh(objName);
		}
	};


	std::vector<model>	mList;
	size_t				mTotla;

public:
	scene();
	~scene();
	scene(const char* scene_file);
	GLuint Loadtexture(const char* tex_file);
};

#endif