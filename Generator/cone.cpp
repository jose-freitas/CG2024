#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

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
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void drawCone(float radius, float height, int slices) {

    int delta = ((2*M_PI)/slices);

    glPolygoMode (GL_FRONT_AND_BACK, GL_LINE);

    glBegin(GL_TRIANGLES);
    for(int i = 0; i<slices; i++)
    {
        float alfa = delta * i;
        float nextalfa = delta * (i+1);

        // Base
        glVertex3f((radius * sin(alfa)) ,0 , (radius * cos(alfa)));
        glVertex3f((radius * sin(nextalfa)),0 , (radius * cos(nextalfa)));
        glVertex3f(0 ,0 ,0 );

        // Lateral
        glVertex3f((radius * sin(alfa)), height, (radius * cos(alfa)));
        glVertex3f((radius * sin(alfa)) ,0 , (radius * cos(alfa)));
        glVertex3f((radius * sin(nextalfa)),0 , (radius * cos(nextalfa)));

    }
    glEnd();

}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(cameraX,cameraY,cameraZ,
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	drawCylinder(1,2,10);

	// End of frame
	glutSwapBuffers();
}
