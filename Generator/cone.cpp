#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <bitset>

int timebase;
float frame;
int verticeCount;
GLuint vertices, verticeCount;

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}

void prepareCone(float radius, float height, int slices) {

    vector<float> p;
    int i;


    int delta = ((2*M_PI)/slices);

    glBegin(GL_TRIANGLES);


    for(int i = 0; i<slices; i++)
    {
        float alfa = delta * i;
        float nextalfa = delta * (i+1);

        // vérice do topo
        p.push_back(0.0f);
        p.push_back(height);
        p.push_back(0.0f);

        // bottom
        p.push_back((radius * sin(alfa)));
        p.push_back(0.0f);
        p.push_back((radius * cos(alfa)));

        p.push_back(0);
        p.push_back(0);
        p.push_back(0);

        p.push_back((radius * sin(nextalfa)));
        p.push_back((0));
        p.push_back((radius * cos(nextalfa)));

        //body
        p.push_back((radius * sin(alfa)));
        p.push_back((height));
        p.push_back((radius * cos(alfa)));

        p.push_back(0.0f);
        p.push_back(height);
        p.push_back(0.0f);

        p.push_back((radius * sin(nextalfa)));
        p.push_back((0));
        p.push_back((radius * cos(nextalfa)));
    }
    glEnd();
    

	verticeCount = p.size() / 3;

	// criar o VBO
    glGenBuffers(1, &vertices);

	glBindBuffer(GL_ARRAY_BUFFER, vertices);
 	glBufferData(GL_ARRAY_BUFFER, 
                    sizeof(float) * p.size(), // tamanho do vector em bytes
 	                p.data(), // os dados do array associado ao vector
 	                GL_STATIC_DRAW); // indicativo da utilização (estático e para desenho)
	
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
 	glVertexPointer(3, GL_FLOAT, 0, 0);
 	glDrawArrays(GL_TRIANGLES, 0, verticeCount);

	prepareCone(1,2,10);
	//cylinder(1,2,10);

    frame++;
	float fps = 0;
    int time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        fps = frame*1000.0/(time-timebase);
        timebase = time;
        frame = 0;
    }

    char buffer[20]; 
    sprintf(buffer, "%.2f", fps);
	glutSetWindowTitle(buffer);

    // End of frame
	glutSwapBuffers();
}
