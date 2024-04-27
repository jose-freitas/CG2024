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
#include "catmullrom.cpp"


float alfa = 0.0f, bet = 0.0f, radius = 20.0f;
float TESSELLATION = 100.0f;

World worldSettings;

int timebase;
float frames;

float lastTime = 0.0f;
float deltaTime = 0.0f;

std::vector<GLuint> renderVertices; // VBOs for groups

float elapsedTime = 0.0f;


void spherical2Cartesian() {
    worldSettings.camera.position.x = radius * cos(bet) * sin(alfa);
    worldSettings.camera.position.y = radius * sin(bet);
    worldSettings.camera.position.z = radius * cos(bet) * cos(alfa);
}

void initVBO(Group& group) {
    if (!group.groupVertices.empty())
    {
        int vertexCount = group.groupVertices.size() / 3;
        GLuint renderVerticesId;
        glGenBuffers(1, &renderVerticesId);
        glBindBuffer(GL_ARRAY_BUFFER, renderVerticesId);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 3, group.groupVertices.data(), GL_STATIC_DRAW);
        group.renderVertices = renderVerticesId; // Store the VBO ID in the group
        renderVertices.push_back(renderVerticesId); // Store the VBO ID in the global vector
    }

    // Recursively initialize VBOs for children groups
    for (int i = 0; i < group.children.size(); i++) {
        initVBO(group.children[i]);
    }
    
}

void renderCatmullRomCurve(float **p, int point_count) {

    float pos[3];
    float deriv[3];

    // draw curve using line segments with GL_LINE_LOOP
    glBegin(GL_LINE_LOOP);
    float gt = 0;
    while (gt < 1) {
        getGlobalCatmullRomPoint(gt, pos, deriv, p, point_count);
        glVertex3f(pos[0],pos[1],pos[2]);
        gt += 1.0 / TESSELLATION;
    }
    glEnd();

}

Coords animateCatmullRomCurve(float time, float **p, int point_count) {

    float pos[3];
    float deriv[3];

    float gt = time > 0.0f ? (elapsedTime / time) : 0.0f;
    getGlobalCatmullRomPoint(gt, pos, deriv, p, point_count);

    return Coords { pos[0], pos[1], pos[2] };
}

void TransformGroup(Group& group)
{
    // Transform Translate points to Multidimensional Array
    int point_count = group.transform.translate.points.size();

    float** p = new float*[point_count];

    for(int i = 0; i < point_count; i++) {
        Coords point = group.transform.translate.points[i];
        p[i] = new float[3];
        p[i][0] = point.x;
        p[i][1] = point.y;
        p[i][2] = point.z;
    }

    // Animate Curve
    Coords newTranslate = group.transform.currentTranslate;

    if (point_count >= 4) {
        newTranslate = animateCatmullRomCurve(group.transform.translate.time, p, point_count);
    }

    // Animate Rotation
    float angle = group.transform.rotate.angle;
    
    if (group.transform.rotate.time > 0.0f)
        angle = elapsedTime * 360.0f / group.transform.rotate.time;
    
    // Transform
    glTranslatef(newTranslate.x, newTranslate.y, newTranslate.z);
    glRotatef(angle, group.transform.rotate.point.x, group.transform.rotate.point.y, group.transform.rotate.point.z);
    glScalef(group.transform.scale.x, group.transform.scale.y, group.transform.scale.z);
}

void renderGroup(Group& group, vector<Group *> parentGroups) {
    
    elapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Convert to seconds

    int vertexCount = group.groupVertices.size() / 3;

    // Transform Translate points to Multidimensional Array
    int point_count = group.transform.translate.points.size();

    float** p = new float*[point_count];

    for(int i = 0; i < point_count; i++) {
        Coords point = group.transform.translate.points[i];
        p[i] = new float[3];
        p[i][0] = point.x;
        p[i][1] = point.y;
        p[i][2] = point.z;
    }

    // Apply Transforms to Curves
    glPushMatrix();

    glColor3f(1.0f, 1.0f, 1.0f); // White for now

    // Apply all previous parent transforms
    for (int i = 0; i < parentGroups.size(); i++)
    {
        Group& parentGroup = *parentGroups[i];
        TransformGroup(parentGroup);
    }

    // Draw Curve
    if (point_count >= 4) {
        renderCatmullRomCurve(p, point_count);
    }

    TransformGroup(group);

    // Draw Shapes
    if (!group.groupVertices.empty()) { // Check if vertices exist
        glBindBuffer(GL_ARRAY_BUFFER, group.renderVertices);
        glVertexPointer(3, GL_FLOAT, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    // Clean Transform Matrix
    glPopMatrix();

    parentGroups.push_back(&group);
    
    // Render Group Children
    for(int i = 0; i < group.children.size(); i++) {
        renderGroup(group.children[i], parentGroups);
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

    Coords emptyCoords = Coords { 0.0f, 0.0f, 0.0f };

    Transform baseTransform = Transform {
        currentTranslate: emptyCoords,
        translate: Translate { points: { emptyCoords, emptyCoords, emptyCoords, emptyCoords } },
        rotate: Rotate { angle: 0.0f, time: 0.0f, point: emptyCoords },
        scale: Coords { 1.0f, 1.0f, 1.0f }
    };

    //renderGroup(worldSettings.root, baseTransform);
    vector<Group*> rootParent;
    renderGroup(worldSettings.root, rootParent);

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

	// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(worldSettings.windowWidth, worldSettings.windowHeight);
    glutCreateWindow("CG@DI-UM");

	// Required callback registry 
	timebase = glutGet (GLUT_ELAPSED_TIME);

    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
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
