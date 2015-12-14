// light.h: interface for the light class.
// author: 郭大为（David Guo)
// time  : 2015/10/18
// func  : read light data from file *.light to class light
//////////////////////////////////////////////////////////////////////

#ifndef AFX_LIGHT_H_
#define AFX_LIGHT_H_

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>

class lightsrc {

public:

	class light_point {
	public:
		float light_position[4];
		float light_ambient[3];
		float light_diffuse[3];
		float light_specular[3];
		
	public:
		/* 构造函数 light_point(posi, ambi, difu, spce) */
		light_point(float *posi, float *ambi, float *difu, float *spec) {
			for (size_t i = 0; i < 3; i++) {
				light_specular[i] = spec[i];
				light_diffuse[i]  = difu[i];
				light_ambient[i]  = ambi[i];
				light_position[i] = posi[i];
			}
			light_position[3] = posi[3];
		}

	};

	float						env_ambient[3];		// define the ambient light from environment. rgb
	std::vector<light_point>	lightList;
	size_t						lTotal;				// Number of light


public:
	lightsrc();
	lightsrc(const char* view_file);
	~lightsrc();

};

#endif