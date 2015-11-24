// mesh.cpp: implementation of the mesh class.
//
//////////////////////////////////////////////////////////////////////

#include "mesh.h"
#include <iostream>

const char* obj_database = "";	// 定義 mesh 的預設目錄

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mesh::mesh(const char* obj_file)
{
	mTotal = 0;		// mList[0] reserved for default meterial
	vTotal = tTotal = nTotal = fTotal = 0;
	
	Init(obj_file);
}

mesh::mesh()
{
	mTotal = 0;			
	vTotal = tTotal = nTotal = fTotal = 0;
}

mesh::~mesh()
{
}

void mesh::LoadMesh(string obj_file)
{
	FILE	*scene;
	char	token[100], buf[100], v[5][100];	// v[5] 表示一個 polygon 最多可以有 5個 vertex
	float	vec[3];

	size_t	n_vertex, n_texture, n_normal;
	size_t	cur_mtl = 0;				// state variable: 目前所使用的 material

	scene = fopen(obj_file.c_str(),"r");
	s_file = obj_file;

	if (!scene) 
	{
		cout<< string("Can not open object File \"") << obj_file << "\" !" << endl;
		return;
	}

	cout<<endl<<obj_file<<endl;
		
	while(!feof(scene))
	{
		token[0] = NULL;
		fscanf(scene,"%s", token);		// 讀 token

		if (!strcmp(token,"g"))
		{
			fscanf(scene,"%s",buf);
		}

		else if (!strcmp(token,"mtllib"))
		{
			char mat_file[256];
			matFile = mat_file;
  			fscanf(scene,"%s", mat_file);
			LoadMtl(string(obj_database) + string(mat_file));
		}

		else if (!strcmp(token,"usemtl"))
		{
			fscanf(scene,"%s",buf);
			cur_mtl = matMap[s_file+string("_")+string(buf)];
		}

		else if (!strcmp(token,"v"))
		{
			fscanf(scene,"%f %f %f",&vec[0],&vec[1],&vec[2]);
			vList.push_back(Vec3(vec));
		}

		else if (!strcmp(token,"vn"))
		{
			fscanf(scene,"%f %f %f",&vec[0],&vec[1],&vec[2]);
			nList.push_back(Vec3(vec));
		}
		else if (!strcmp(token,"vt"))
		{
			fscanf(scene,"%f %f",&vec[0],&vec[1]);
			tList.push_back(Vec3(vec));
		}

		else if (!strcmp(token,"f"))
		{
			size_t i;
			for (i=0;i<3;i++)		// face 預設為 3，假設一個 polygon 都只有 3 個 vertex
			{
				fscanf(scene,"%s",v[i]);
				//printf("[%s]",v[i]);
			}
			//printf("\n");
		  
			Vertex	tmp_vertex[3];		// for faceList structure

			for (i=0;i<3;i++)		// for each vertex of this face
			{
				char str[20], ch;
				size_t base,offset;
				base = offset = 0;

				// calculate vertex-list index
				while( (ch=v[i][base+offset]) != '/' && (ch=v[i][base+offset]) != '\0')
				{
					str[offset] = ch;
					offset++;
				}
				str[offset] = '\0';
				n_vertex = atoi(str);
				base += (ch == '\0')? offset : offset+1;
				offset = 0;

				// calculate texture-list index
				while( (ch=v[i][base+offset]) != '/' && (ch=v[i][base+offset]) != '\0')
				{
					str[offset] = ch;
					offset++;
				}
				str[offset] = '\0';
				n_texture = atoi(str);	// case: xxx//zzz，texture 設為 0 (tList 從 1 開始)
				base += (ch == '\0')? offset : offset+1;
				offset = 0;

				// calculate normal-list index
				while( (ch=v[i][base+offset]) != '\0')
				{
					str[offset] = ch;
					offset++;
				}
				str[offset] = '\0';
				n_normal = atoi(str);	// case: xxx/yyy，normal 設為 0 (nList 從 1 開始)

				tmp_vertex[i].v = n_vertex;
				tmp_vertex[i].t = n_texture;
				tmp_vertex[i].n = n_normal;
			}

			faceList.push_back(FACE(tmp_vertex[0],tmp_vertex[1],tmp_vertex[2], cur_mtl));
		}

		else if (!strcmp(token,"#"))	  // 註解
			fgets(buf,100,scene);

//		printf("[%s]\n",token);
	}

	if (scene) fclose(scene);

	vTotal = vList.size();
	nTotal = nList.size();
	tTotal = tList.size();
	fTotal = faceList.size();
	printf("vetex: %d, normal: %d, texture: %d, triangles: %d\n",vTotal, nTotal, tTotal, fTotal);
}

void mesh::LoadMtl(string tex_file)
{
	char	token[100], buf[100];
	float	r,g,b;

	fp_mtl = fopen(tex_file.c_str(),"r");

	if (!fp_mtl) 
	{
		cout << "Can't open material file \"" << tex_file << "\"!" << endl;
		return;
	}

	cout<<tex_file<<endl;

	size_t cur_mat;

	while(!feof(fp_mtl))
	{
		token[0] = NULL;
		fscanf(fp_mtl,"%s", token);		// 讀 token

		if (!strcmp(token,"newmtl"))
		{
			fscanf(fp_mtl,"%s",buf);
			material newMtl;
			mList.push_back(newMtl);
			cur_mat = mTotal++;					// 從 mList[1] 開始，mList[0] 空下來給 default material 用
			matMap[s_file+string("_")+string(buf)] = cur_mat; 	// matMap["material_name"] = material_id;
		}

		else if (!strcmp(token,"Ka"))
		{
			fscanf(fp_mtl,"%f %f %f",&r,&g,&b);
			mList[cur_mat].Ka[0] = r;
			mList[cur_mat].Ka[1] = g;
			mList[cur_mat].Ka[2] = b;
			mList[cur_mat].Ka[3] = 1;
		}

		else if (!strcmp(token,"Kd"))
		{
			fscanf(fp_mtl,"%f %f %f",&r,&g,&b);
			mList[cur_mat].Kd[0] = r;
			mList[cur_mat].Kd[1] = g;
			mList[cur_mat].Kd[2] = b;
			mList[cur_mat].Kd[3] = 1;
		}

		else if (!strcmp(token,"Ks"))
		{
			fscanf(fp_mtl,"%f %f %f",&r,&g,&b);
			mList[cur_mat].Ks[0] = r;
			mList[cur_mat].Ks[1] = g;
			mList[cur_mat].Ks[2] = b;
			mList[cur_mat].Ks[3] = 1;
		}

		else if (!strcmp(token,"Ns"))
		{
			fscanf(fp_mtl,"%f",&r);
			mList[cur_mat].Ns = r;
		}

		else if (!strcmp(token,"Tr"))
		{
			fscanf(fp_mtl,"%f",&r);
			mList[cur_mat].Tr = r;
		}

		else if (!strcmp(token,"d"))
		{
			fscanf(fp_mtl,"%f",&r);
			mList[cur_mat].Tr = r;
		}

		if (!strcmp(token,"map_Kd"))
		{
			fscanf(fp_mtl,"%s",buf);
			mList[cur_mat].map_Kd = buf;
		}

		if (!strcmp(token,"map_Ks"))
		{
			fscanf(fp_mtl,"%s",buf);
			mList[cur_mat].map_Ks = buf;
		}

		if (!strcmp(token,"map_Ka"))
		{
			fscanf(fp_mtl,"%s",buf);
			mList[cur_mat].map_Ka = buf;
		}

		else if (!strcmp(token,"#"))	  // 註解
			fgets(buf,100,fp_mtl);

//		printf("[%s]\n",token);
	}

	printf("total material:%d\n",matMap.size());

	if (fp_mtl) fclose(fp_mtl);
}

void mesh::Init(const char* obj_file)
{
	float default_value[3] = {1,1,1};

	vList.push_back(Vec3(default_value));	// 因為 *.obj 的 index 是從 1 開始
	nList.push_back(Vec3(default_value));	// 所以要先 push 一個 default value 到 vList[0],nList[0],tList[0]
	tList.push_back(Vec3(default_value));

	material defaultMtl;
	mList.push_back(defaultMtl);
	// 定義 default meterial: mList[0]
	mList[0].Ka[0] = 0.0f; mList[0].Ka[1] = 0.0f; mList[0].Ka[2] = 0.0f; mList[0].Ka[3] = 1.0f; 
	mList[0].Kd[0] = 1.0f; mList[0].Kd[1] = 1.0f; mList[0].Kd[2] = 1.0f; mList[0].Kd[3] = 1.0f; 
	mList[0].Ks[0] = 0.8f; mList[0].Ks[1] = 0.8f; mList[0].Ks[2] = 0.8f; mList[0].Ks[3] = 1.0f;
	mList[0].Ns = 32.0f;
	mTotal++;

	LoadMesh(string(obj_file));		// 讀入 .obj 檔 (可處理 Material)
}

