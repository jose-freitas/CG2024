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

#include <IL/il.h>

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

float elapsedTime = 0.0f;


void checkGLError(const char* stmt, const char* fname, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        //std::cerr << "OpenGL error " << err << ", at " << fname << ":" << line << " - for " << stmt << std::endl; // UNCOMMENT TO DEBUG OPENGL
        // Optionally, exit or abort the program
    }
}

#define GL_CHECK(stmt) do { \
    stmt; \
    checkGLError(#stmt, __FILE__, __LINE__); \
} while (0)


void spherical2Cartesian() {
    worldSettings.camera.position.x = radius * cos(bet) * sin(alfa);
    worldSettings.camera.position.y = radius * sin(bet);
    worldSettings.camera.position.z = radius * cos(bet) * cos(alfa);
}

int loadTexture(std::string s) {

	unsigned int t,tw,th;
	unsigned char *texData;
	unsigned int texID;

	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilGenImages(1,&t); 
	ilBindImage(t);
	ilLoadImage((ILstring)s.c_str());
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();

	glGenTextures(1,&texID);
	
	glBindTexture(GL_TEXTURE_2D,texID);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,		GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,		GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MAG_FILTER,   	GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;

}

void initVBO(Group& group) {
    // LOOP THROUGH EACH MODEL
    for (int i = 0; i < group.models.size(); i++) 
    {
        Model& model = group.models[i];
        
        // INIT MODEL VBOs
        if (!model.modelData.modelVertices.empty())
        {
            int vertexCount = model.modelData.modelVertices.size() / 3;
            GLuint renderVerticesId;
            GLuint renderNormalsId;
            GLuint renderUvsId;

            glGenBuffers(1, &renderVerticesId);
            glBindBuffer(GL_ARRAY_BUFFER, renderVerticesId);
            glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 3, model.modelData.modelVertices.data(), GL_STATIC_DRAW);

            glGenBuffers(1, &renderNormalsId);
            glBindBuffer(GL_ARRAY_BUFFER, renderNormalsId);
            glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 3, model.modelData.modelNormals.data(),  GL_STATIC_DRAW); 

            glGenBuffers(1, &renderUvsId);
            glBindBuffer(GL_ARRAY_BUFFER, renderUvsId);
            glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 2, model.modelData.modelUvs.data(),  GL_STATIC_DRAW); 

            // Store the VBO ID in the model
            model.renderVertices = renderVerticesId; 
            model.renderNormals = renderNormalsId; 
            model.renderUvs = renderUvsId; 
        }

        // INIT MODEL TEXTURE
        if(model.texture.file != "")
        {
            model.texture.texID = loadTexture(model.texture.file);
        }
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

    // LOOP THROUGH EACH MODEL
    for (int i = 0; i < group.models.size(); i++) 
    {
        Model model = group.models[i];

        int vertexCount = model.modelData.modelVertices.size() / 3;

        // DRAW MODEL
        if (!model.modelData.modelVertices.empty()) { // Check if vertices exist

            float shiny[1] = { (float) model.color.shininess };

	        glMaterialfv(GL_FRONT, GL_AMBIENT, model.color.ambient.ToFloats());
            glMaterialfv(GL_FRONT, GL_DIFFUSE, model.color.diffuse.ToFloats());
            glMaterialfv(GL_FRONT, GL_SPECULAR, model.color.specular.ToFloats());
            glMaterialfv(GL_FRONT, GL_EMISSION, model.color.emissive.ToFloats());
            glMaterialfv(GL_FRONT, GL_SHININESS, shiny);

            glBindTexture(GL_TEXTURE_2D, model.texture.texID);

            GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, model.renderVertices));
            GL_CHECK(glVertexPointer(3, GL_FLOAT, 0, 0));

            GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, model.renderNormals));
            GL_CHECK(glNormalPointer(GL_FLOAT,0,0));

            GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, model.renderUvs));
            GL_CHECK(glTexCoordPointer(2,GL_FLOAT,0,0));

            GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, vertexCount));

            glBindTexture(GL_TEXTURE_2D, 0);
        }

    }
    
    // Clean Transform Matrix
    glPopMatrix();

    parentGroups.push_back(&group);

    // Render Group Children
    for(int i = 0; i < group.children.size(); i++) {
        renderGroup(group.children[i], parentGroups);
    }
}

void renderLights(std::vector<Light> lights)
{
    for(int i = 0; i < GL_MAX_LIGHTS; i++) 
    {
        if ((i + 1) > lights.size())
            return;

        Light light = lights[i];

        if(light.type == "point")
        {
            glLightfv(i , GL_POSITION, light.position.ToFloats());
        }
        else if(light.type == "directional")
        {
            glLightfv(i , GL_POSITION, light.direction.ToFloats());
        }
        else if(light.type == "spotlight")
        {
            float cuty[1] = { light.cutoff };

            glLightfv(i , GL_POSITION, light.position.ToFloats());
            glLightfv(i , GL_SPOT_DIRECTION, light.direction.ToFloats());
            glLightfv(i , GL_SPOT_CUTOFF, cuty);
        }
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
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the Camera
    setCamera(worldSettings.camera);

    // Rendering
    renderAxis();

    // Set the Lights
    renderLights(worldSettings.lights);

    Coords emptyCoords = Coords { 0.0f, 0.0f, 0.0f };

    Transform baseTransform = Transform {
        currentTranslate: emptyCoords,
        translate: Translate { points: { emptyCoords, emptyCoords, emptyCoords, emptyCoords } },
        rotate: Rotate { angle: 0.0f, time: 0.0f, point: emptyCoords },
        scale: Coords { 1.0f, 1.0f, 1.0f }
    };

    Texture baseTexture = Texture {/*base values*/};
    Light baseLight = {/*base values*/};

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

    // Callback registration
    glutSpecialFunc(processSpecialKeys);

    #ifndef _APPLE
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    #endif


    // Init VBO
    initVBO(worldSettings.root);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

    glEnable(GL_RESCALE_NORMAL); // check

    // init Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float dark[4] = {0.2, 0.2, 0.2, 1.0};
	float white[4] = {1.0, 1.0, 1.0, 1.0};
	float black[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    // light colors
	glLightfv(GL_LIGHT0, GL_AMBIENT, dark);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    // controls global ambient light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);

    float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

    //init
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //Texturas
    glEnable(GL_TEXTURE_2D);

    //texIDFigura = loadTexture("NomeDaTextura.jpg");

    printInfo();

    glutMainLoop();

    return 1; 
}
