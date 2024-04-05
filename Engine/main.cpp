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


float alfa = 0.0f, bet = 0.0f, radius = 5.0f;

World worldSettings;

int timebase;
float frames;

std::vector<GLuint> renderVertices; // VBOs for groups


void spherical2Cartesian() {
    worldSettings.camera.position.x = radius * cos(bet) * sin(alfa);
    worldSettings.camera.position.y = radius * sin(bet);
    worldSettings.camera.position.z = radius * cos(bet) * cos(alfa);
}

void initVBO(Group& group) {
    if (group.groupVertices.empty())
        return; // Skip initialization if group has no vertices

    int vertexCount = group.groupVertices.size() / 3;
    GLuint renderVerticesId;
    glGenBuffers(1, &renderVerticesId);
    glBindBuffer(GL_ARRAY_BUFFER, renderVerticesId);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 3, group.groupVertices.data(), GL_STATIC_DRAW);
    group.renderVertices = renderVerticesId; // Store the VBO ID in the group
    renderVertices.push_back(renderVerticesId); // Store the VBO ID in the global vector

    // Recursively initialize VBOs for children groups
    for (int i = 0; i < group.children.size(); i++) {
        initVBO(group.children[i]);
    }
}

void renderGroup(Group& group, Transform parentTransform) {
    int vertexCount = group.groupVertices.size() / 3;

	// Calculate new Parent Transform
    Transform newTransform = Transform {
        translate: parentTransform.translate.Add(group.transform.translate),
        rotate: group.transform.rotate,
        rotateAngle: parentTransform.rotateAngle + group.transform.rotateAngle,
        scale: parentTransform.scale.Mult(group.transform.scale)
    };

    // Apply Transform
    glPushMatrix();

    glColor3f(1.0f, 1.0f, 1.0f); // White for now
    glTranslatef(newTransform.translate.x, newTransform.translate.y, newTransform.translate.z);
	glScalef(newTransform.scale.x, newTransform.scale.y, newTransform.scale.z);
	glRotatef(newTransform.rotateAngle, newTransform.rotate.x, newTransform.rotate.y, newTransform.rotate.z);

    // Draw Shapes
    if (!group.groupVertices.empty()) { // Check if vertices exist
        glBindBuffer(GL_ARRAY_BUFFER, group.renderVertices);
        glVertexPointer(3, GL_FLOAT, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    // Clean Transform Matrix
    glPopMatrix();

    // Render Group Children
    for(int i = 0; i < group.children.size(); i++) {
        renderGroup(group.children[i], newTransform);
    }
}

void renderAxis () {
    glBegin(GL_LINES);
        // X axis in red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-100.0f, 0.0f, 0.0f);
        glVertex3f( 100.0f, 0.0f, 0.0f);
        // Y Axis in Green
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -100.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);
        // Z Axis in Blue
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -100.0f);
        glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
}

void renderScene() {
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the Camera
    setCamera(worldSettings.camera);

    // Rendering
    renderAxis();
    
    Transform baseTransform = Transform {
        translate: Coords { 0.0f, 0.0f, 0.0f },
        rotate: Coords { 0.0f, 0.0f, 0.0f },
        rotateAngle: 0.0f,
        scale: Coords { 1.0f, 1.0f, 1.0f }
    };

    renderGroup(worldSettings.root, baseTransform);

    // End of frame
    glutSwapBuffers();
}

void processSpecialKeys(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            alfa -= 0.1;
            break;

        case GLUT_KEY_LEFT:
            alfa += 0.1;
            break;

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

        case GLUT_KEY_PAGE_DOWN:
            radius -= 0.1f;
            if (radius < 0.1f)
                radius = 0.1f;
            break;

        case GLUT_KEY_PAGE_UP:
            radius += 0.1f;
            break;
    }
    spherical2Cartesian();
    glutPostRedisplay();
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

void printInfo() {
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));

    printf("\nUse Arrows to move the camera up/down and left/right\n");
    printf("Page Up and Page Down control the distance from the camera to the origin\n");
}

int main(int argc, char **argv) {

	// Parse XML Settings
    parser("../settings.xml", worldSettings);

    // Force Root Scale always be (1, 1, 1)
    Coords rootScale = worldSettings.root.transform.scale; 

    if (rootScale.x == 0.0f || rootScale.y == 0.0f || rootScale.z == 0.0f) {
        worldSettings.root.transform.scale = Coords { 1.0f, 1.0f, 1.0f };
    }

	// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(worldSettings.windowWidth, worldSettings.windowHeight);
    glutCreateWindow("CG@DI-UM");

	// Required callback registry 
	timebase = glutGet (GLUT_ELAPSED_TIME);

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutSpecialFunc(processSpecialKeys);

	#ifndef _APPLE
		glewInit();
	#endif

	glEnableClientState(GL_VERTEX_ARRAY);

    // Init VBO
    initVBO(worldSettings.root);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);

    printInfo();

    glutMainLoop();

    return 0;
}
