// view.h: interface for the view class.
// author: ¹ù´óÎª£¨David Guo)
// time  : 2015/10/18
// func  : read view data from file *.view for class view
//////////////////////////////////////////////////////////////////////

#ifndef AFX_VIEM_H_
#define AFX_VIEW_H_

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>

class view {

public :
	float eye[3];		// define the position of the camera
	float vat[3];		// define the position where the camera looks at
	float vup[3];		// define the up vector of the camera
	float fovy;			// define the fovy angle of the view frustum
	float dnear;		// define the distance of the near plane from the camera
	float dfar;			// define the distance of the far plane from the camera
	float viewport[4];	// define the position and the size of the view port

public:
	view();
	view(const char* view_file);
	~view();

};



#endif