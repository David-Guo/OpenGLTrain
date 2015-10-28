#include "light.h"
#include <iostream>

using namespace std;

lightsrc::lightsrc() {
	lTotal = 0;
}

lightsrc::~lightsrc() {
}

lightsrc::lightsrc(const char* light_file) {
	FILE *lightfd;
	char token[100];
	float posi[4];
	float ambi[3];
	float difu[3];
	float spec[3];

	lightfd = fopen(light_file, "r");
	if (!lightfd) {
		cout<< string("Can not open view File \"") << lightfd << "\" !" << endl;
		return;
	}

	cout << endl << light_file << endl;

	while (!feof(lightfd)) {
		token[0] = NULL;

		fscanf(lightfd, "%s", token);

		if (!strcmp(token, "light")) {
			
			fscanf(lightfd, "%f %f %f %f %f %f %f %f %f %f %f %f",
				&posi[0], &posi[1], &posi[2], &ambi[0], &ambi[1], &ambi[2],
				&difu[0], &difu[1], &difu[2], &spec[0], &spec[1], &spec[2]);

			/* !!! set the w of light as 1.0 to make it a point light source */
			posi[3] = 1.0;
			lightList.push_back(light_point(posi, ambi, difu, spec));

		}
		else if (!strcmp(token, "ambient")) {
			fscanf(lightfd, "%f %f %f", &env_ambient[0], &env_ambient[1], &env_ambient[2]);
		}

	}

	lTotal = lightList.size();
	fclose(lightfd);

}