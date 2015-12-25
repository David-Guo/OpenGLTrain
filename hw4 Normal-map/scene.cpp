#include "scene.h"

scene::scene(const char* scene_file) {

	FILE *scenefd;
	char modelname[100];
	char texturefile0[50];
	char texturefile1[50];
	char texturefile2[50];
	char texturefile3[50];
	char texturefile4[50];
	char texturefile5[50];
	char token[100];
	float scale[3];
	float rotate[4];
	float translate[3];

	GLuint texObject1 = 0;
	GLuint texObject2 = 0;
	GLuint texObject3 = 0;

	scenefd = fopen(scene_file, "r");
	
	while (!feof(scenefd)) {
		token[0] = NULL;

		fscanf(scenefd, "%s", token);

		if (!strcmp(token, "shader_textures")) {
			fscanf(scenefd, "%s", texturefile0);
			texObject1 = Loadtexture(texturefile0);
			fscanf(scenefd, "%s", texturefile1);
			texObject2 = Loadtexture(texturefile1);
			fscanf(scenefd, "%s", texturefile2);
			texObject3 = Loadtexture(texturefile2);
			
		}
		else if (!strcmp(token, "model")) {
			fscanf(scenefd, "%s", modelname);
			fscanf(scenefd, "%f %f %f", &scale[0], &scale[1], &scale[2]);
			fscanf(scenefd, "%f %f %f %f", &rotate[0], &rotate[1], &rotate[2], &rotate[3]);
			fscanf(scenefd, "%f %f %f", &translate[0], &translate[1], &translate[2]);

			model tempModel = model(modelname, scale, rotate, translate);
			tempModel.ambTextureId = texObject1;
			tempModel.difTextureId = texObject2;
			tempModel.spcTextureId = texObject3;
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
	
	FreeImage_Initialise();
	GLuint texObject = 0;
	glGenTextures(1, &texObject);
	int error = glGetError();
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
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return texObject;
	FreeImage_DeInitialise();
}
