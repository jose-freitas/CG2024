#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void setCamera(float camX, float camY, float camZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ, float fov, float near, float far) {
	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 
		      lookX, lookY, lookZ,
			  upX, upY, upZ);
	gluPerspective(fov, 0, near, far);
}