#include <stdio.h>
#include <vector>
#include "./include/parser.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>


void setCamera(Camera camera) {
	glLoadIdentity();
	gluLookAt(camera.position.x, camera.position.y, camera.position.z, 
		      camera.lookAt.x, camera.lookAt.y, camera.lookAt.z,
			  camera.up.x, camera.up.y, camera.up.z);
	//gluPerspective(camera.projection.fov, 1.0, camera.projection.near, camera.projection.far);
}