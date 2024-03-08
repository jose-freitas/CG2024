#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include "parser.cpp"
#include "camera.cpp"


const std::string MODEL_PATH = "../../modelFiles/";

float alfa = 0.0f, bet = 0.0f, radius = 5.0f;

World worldSettings;

int timebase;
float frames;

GLuint renderVertices, verticeCount; 



std::vector<float> verticesGlobal;


std::vector<float> createModelsArray(std::vector<std::string> filenames){
	std::vector<float> vertices;

	cout << "Loading .3d files..." << "\n";

	for(int i = 0; i < filenames.size(); i++){


		std::string filename = filenames[i];

		cout << filename << "\n";

		std::ifstream file = std::ifstream(MODEL_PATH + filename);

		float coordinate;

		while(file >> coordinate){
			//cout << coordinate << "\n"; // DEBUG
			vertices.push_back(coordinate);
		}

		file.close();
	}

	cout << "Finished loading .3d files!" << "\n";

	return vertices;
}

void spherical2Cartesian() {
	
	worldSettings.camera.position.x = radius * cos(bet) * sin(alfa);
	worldSettings.camera.position.y = radius * sin(bet);
	worldSettings.camera.position.z = radius * cos(bet) * cos(alfa);
}


void renderScene(void) {

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set the Camera
	/*glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);*/
	setCamera(worldSettings.camera);

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

void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		bet += 0.1f;
		if (bet > 1.5f)
			bet = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		bet -= 0.1f;
		if (bet < -1.5f)
			bet = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 0.1f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Page Up and Page Down control the distance from the camera to the origin\n");
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	//gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);
	Camera camera = worldSettings.camera;
	gluPerspective(camera.projection.fov, ratio, camera.projection.near, camera.projection.far);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {

	std::vector<std::string> filenames;

	parser("../settings.xml", worldSettings);

	for(int i = 0; i < worldSettings.models.size(); i++){
		filenames.push_back(worldSettings.models[i].mod);
	}

	verticesGlobal = createModelsArray(filenames);

// init GLUT and the window

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(worldSettings.windowWidth,worldSettings.windowHeight);
	glutCreateWindow("CG@DI-UM");

	glEnableClientState(GL_VERTEX_ARRAY);
		
// Required callback registry 
	//spherical2Cartesian();
	timebase = glutGet (GLUT_ELAPSED_TIME);

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutIdleFunc(renderScene);
	
//Callback registration for keyboard processing
	glutSpecialFunc(processSpecialKeys);
	

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