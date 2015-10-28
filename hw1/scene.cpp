#include "scene.h"


scene::scene(const char* scene_file) {

	FILE *scenefd;
	char modelname[100];
	char token[100];
	float scale[3];
	float rotate[4];
	float translate[3];

	scenefd = fopen(scene_file, "r");

	while (!feof(scenefd)) {
		token[0] = NULL;

		fscanf(scenefd, "%s", token);

		if (!strcmp(token, "model")) {
			fscanf(scenefd, "%s", modelname);
			fscanf(scenefd, "%f %f %f", &scale[0], &scale[1], &scale[2]);
			fscanf(scenefd, "%f %f %f %f", &rotate[0], &rotate[1], &rotate[2], &rotate[3]);
			fscanf(scenefd, "%f %f %f", &translate[0], &translate[1], &translate[2]);
		}
		mList.push_back(model(scale, rotate, translate));
	}

	mTotla = mList.size();
	fclose(scenefd);
}

scene::scene() {
}

scene::~scene() {
}

