#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "parser.cpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

int timebase;
float frames;

GLuint renderVertices, verticeCount; 


std::vector<float> verticesGlobal;


std::vector<float> createModelsArray(std::vector<std::string> filenames){
	std::vector<float> vertices;

	for(int i = 0; i < filenames.size(); i++){

		std::string filename = filenames[i];
		std::ifstream file = std::ifstream(filename);

		float coordinate;

		while(file >> coordinate){
			vertices.push_back(coordinate);
		}

		file.close();
	}
	return vertices;
}


void renderScene(void) {

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	verticeCount = verticesGlobal.size() / 3; 

	//criar o VBO
	glGenBuffers(1, &renderVertices);

	//copiar o vector para a memória gráfica
	glBindBuffer(GL_ARRAY_BUFFER, renderVertices);
	glBufferData(
		GL_ARRAY_BUFFER, 
		sizeof(float) * verticesGlobal.size(),
		verticesGlobal.data(),
		GL_STATIC_DRAW);

	// Draw Shapes
	glBindBuffer(GL_ARRAY_BUFFER, renderVertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, verticeCount);

	// End of frame
	glutSwapBuffers();
}


void printInfo() {
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Page Up and Page Down control the distance from the camera to the origin\n");
}


int main(int argc, char **argv) {

	World world;

	parser("settings.xml", world);

	verticesGlobal = createModelsArray(world.models);

// init GLUT and the window

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");

	glEnableClientState(GL_VERTEX_ARRAY);
		
// Required callback registry 
	timebase = glutGet (GLUT_ELAPSED_TIME);

	glutDisplayFunc(renderScene);
	//glutReshapeFunc(changeSize);

	glutIdleFunc(renderScene);
	
// Callback registration for keyboard processing
	/*glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);*/
	

	// init GLEW
#ifndef __APPLE__
	glewInit();
#endif


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}