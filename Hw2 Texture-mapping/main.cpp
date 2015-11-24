#include <direct.h>
#include "scene.h"
#include "view.h"
#include "light.h"
#include "glut.h"

view *globalview;
lightsrc *globalight;
scene *globalscene;

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

	//globalview = new view("TestScene1/view.view");
	//globalight = new lightsrc("TestScene1/light.light");
	//globalscene = new scene("TestScene1/scene.scene");
}