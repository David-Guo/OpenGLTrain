#include "view.h"
#include <iostream>

using namespace std;

view::view()
{
}

view::~view()
{
}

view::view(const char* view_file) {
	FILE *viewfd;
	char token[100];

	viewfd = fopen(view_file, "r");
	if (!viewfd) {
		cout<< string("Can not open view File \"") << viewfd << "\" !" << endl;
		return;
	}

	cout << endl << view_file << endl;

	while (!feof(viewfd)) {
		token[0] = NULL;
		
		fscanf(viewfd, "%s", token);

		if (!strcmp(token, "eye")) 
			fscanf(viewfd, "%f %f %f", &eye[0], &eye[1], &eye[2]);
		else if (!strcmp(token, "vat"))
			fscanf(viewfd, "%f %f %f", &vat[0], &vat[1], &vat[2]);
		else if (!strcmp(token, "vup"))
			fscanf(viewfd, "%f %f %f", &vup[0], &vup[1], &vup[2]);
		else if (!strcmp(token, "fovy"))
			fscanf(viewfd, "%f", &fovy);
		else if (!strcmp(token, "dnear"))
			fscanf(viewfd, "%f", &dnear);
		else if (!strcmp(token, "dfar"))
			fscanf(viewfd, "%f", &dfar);
		else if (!strcmp(token, "viewport"))
			fscanf(viewfd, "%f %f %f %f", &viewport[0], &viewport[1], &viewport[2], &viewport[3]);

	}

	fclose(viewfd);

}