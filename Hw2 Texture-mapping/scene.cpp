#include "scene.h"


scene::scene(const char* scene_file) {

	FILE *scenefd;
	char modelname[100];
	char *texturefile[6];
	char token[100];
	float scale[3];
	float rotate[4];
	float translate[3];

	int texWay = 0;
	GLuint texObject1 = NULL;
	GLuint texObject2 = NULL;

	scenefd = fopen(scene_file, "r");

	while (!feof(scenefd)) {
		token[0] = NULL;

		fscanf(scenefd, "%s", token);

		if (!strcmp(token, "no-texture")) {
			texWay = 0;
			texObject1 = NULL;
			texObject2 = NULL;
		}
		else if(!strcmp(token, "single-texture")) {
			fscanf(scenefd, "%s", texturefile[0]);
			texWay = 1;
			texObject1 = Loadtexture(texturefile[0]);
			texObject2 = NULL;
		}
		else if (!strcmp(token, "multi-texture")) {
			texWay = 2;
			fscanf(scenefd, "%s", texturefile[0]);
			texObject1 = Loadtexture(texturefile[0]);
			fscanf(scenefd, "%s", texturefile[1]);
			texObject2 = Loadtexture(texturefile[1]);
		}
		else if (!strcmp(token, "cube-map")) {
			texWay = 3;
			fscanf(scenefd, "%s", texturefile[0]);
			fscanf(scenefd, "%s", texturefile[1]);
			fscanf(scenefd, "%s", texturefile[2]);
			fscanf(scenefd, "%s", texturefile[3]);
			fscanf(scenefd, "%s", texturefile[4]);
			fscanf(scenefd, "%s", texturefile[5]);
			//texObject1 = LoadCubemap(texturefile[0], texturefile[1], texturefile[2], texturefile[3], texturefile[4], texturefile[5]);
			texObject2 = NULL;
		}
		else if (!strcmp(token, "model")) {
			fscanf(scenefd, "%s", modelname);
			fscanf(scenefd, "%f %f %f", &scale[0], &scale[1], &scale[2]);
			fscanf(scenefd, "%f %f %f %f", &rotate[0], &rotate[1], &rotate[2], &rotate[3]);
			fscanf(scenefd, "%f %f %f", &translate[0], &translate[1], &translate[2]);

			model tempModel = model(modelname, scale, rotate, translate);
			tempModel.texWay = texWay;
			tempModel.texObject1 = texObject1;
			tempModel.texObject2 = texObject2;
			mList.push_back(tempModel);
		}
		
	}

	mTotla = mList.size();
	fclose(scenefd);
}

scene::scene() {
}

scene::~scene() {
}

GLuint scene::Loadtexture(const char* pFilename) {
	GLuint texObject;
	glGenTextures(1, &texObject);

	FIBITMAP* pImage = FreeImage_Load(FreeImage_GetFileType(pFilename, 0), pFilename);
	FIBITMAP *p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
	int iWidth = FreeImage_GetWidth(p32BitsImage);
	int iHeight = FreeImage_GetHeight(p32BitsImage);

	glBindTexture(GL_TEXTURE_2D, texObject);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	FreeImage_Unload(p32BitsImage);
	FreeImage_Unload(pImage);

	return texObject;
}
/*
GLint scene::LoadCubemap(char* file_px, char * file_nx, char *file_py, char *file_ny, char *file_pz, char *file_nz) {
	GLuint texObject;
	glGenTextures(1, &texObject);


	FIBITMAP *pImage0 = FreeImage_Load(FreeImage_GetFileType(file_px, 0), file_px);
	FIBITMAP *pImage1 = FreeImage_Load(FreeImage_GetFileType(file_nx, 0), file_nx);
	FIBITMAP *pImage2 = FreeImage_Load(FreeImage_GetFileType(file_py, 0), file_py);
	FIBITMAP *pImage3 = FreeImage_Load(FreeImage_GetFileType(file_ny, 0), file_ny);
	FIBITMAP *pImage4 = FreeImage_Load(FreeImage_GetFileType(file_pz, 0), file_pz);
	FIBITMAP *pImage5 = FreeImage_Load(FreeImage_GetFileType(file_nz, 0), file_nz);

	//FIBITMAP *p32BitsImage[6];

	FIBITMAP *p32BitsImage0 = FreeImage_ConvertTo32Bits(pImage0);
	FIBITMAP *p32BitsImage1 = FreeImage_ConvertTo32Bits(pImage1);
	FIBITMAP *p32BitsImage2 = FreeImage_ConvertTo32Bits(pImage2);
	FIBITMAP *p32BitsImage3 = FreeImage_ConvertTo32Bits(pImage3);
	FIBITMAP *p32BitsImage4 = FreeImage_ConvertTo32Bits(pImage4);
	FIBITMAP *p32BitsImage5 = FreeImage_ConvertTo32Bits(pImage5);


	int iWidth = FreeImage_GetWidth(p32BitsImage0);
	int iHeight = FreeImage_GetHeight(p32BitsImage0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, texObject);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNALED, 
		(void*)FreeImage_GetBits(pImage0));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNALED, 
		(void*)FreeImage_GetBits(pImage1));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNALED, 
		(void*)FreeImage_GetBits(pImage2));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNALED, 
		(void*)FreeImage_GetBits(pImage3));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNALED, 
		(void*)FreeImage_GetBits(pImage4));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNALED, 
		(void*)FreeImage_GetBits(pImage5));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	FreeImage_Unload(p32BitsImage0);
	FreeImage_Unload(p32BitsImage1);
	FreeImage_Unload(p32BitsImage2);
	FreeImage_Unload(p32BitsImage3);
	FreeImage_Unload(p32BitsImage4);
	FreeImage_Unload(p32BitsImage5);
	FreeImage_Unload(pImage0);
	FreeImage_Unload(pImage1);
	FreeImage_Unload(pImage2);
	FreeImage_Unload(pImage3);
	FreeImage_Unload(pImage4);
	FreeImage_Unload(pImage5);


	return texObject;
}*/