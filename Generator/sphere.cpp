#include <GL/glut.h>
#include <vector>
#include <cmath>

#define _USE_MATH_DEFINES

using namespace std;

int verticeCount;
GLuint vertices;

void prepareSphere(float radius, int slices, int stacks) {
    vector<float> p;

    float deltaPhi = (float)(M_PI) / stacks;
    float deltaTheta = (float)(2 * M_PI) / slices;

    for (int i = 0; i < stacks; i++) {
        float phi = i * deltaPhi;
        float nextPhi = (i + 1) * deltaPhi;

        for (int j = 0; j < slices; j++) {
            float theta = j * deltaTheta;
            float nextTheta = (j + 1) * deltaTheta;

            // Primeiro triângulo do quadrilátero
            p.push_back(radius * sin(phi) * cos(theta));
            p.push_back(radius * cos(phi));
            p.push_back(radius * sin(phi) * sin(theta));

            p.push_back(radius * sin(phi) * cos(nextTheta));
            p.push_back(radius * cos(phi));
            p.push_back(radius * sin(phi) * sin(nextTheta));

            p.push_back(radius * sin(nextPhi) * cos(theta));
            p.push_back(radius * cos(nextPhi));
            p.push_back(radius * sin(nextPhi) * sin(theta));

            // Segundo triângulo do quadrilátero
            p.push_back(radius * sin(nextPhi) * cos(theta));
            p.push_back(radius * cos(nextPhi));
            p.push_back(radius * sin(nextPhi) * sin(theta));

            p.push_back(radius * sin(phi) * cos(nextTheta));
            p.push_back(radius * cos(phi));
            p.push_back(radius * sin(phi) * sin(nextTheta));

            p.push_back(radius * sin(nextPhi) * cos(nextTheta));
            p.push_back(radius * cos(nextPhi));
            p.push_back(radius * sin(nextPhi) * sin(nextTheta));
        }
    }

    verticeCount = p.size() / 3;
    // criar o VBO
    glGenBuffers(1, &vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vertices);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * p.size(), 
                 p.data(),                 
                 GL_STATIC_DRAW);          
}

void renderScene(void) {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(5.0f, 5.0f, 5.0f,
              0.0, 0.0, 0.0,
              0.0f, 1.0f, 0.0f);

    glBindBuffer(GL_ARRAY_BUFFER, vertices);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, verticeCount);

    // End of frame
    glutSwapBuffers();
}

/*int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Sphere Example");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(renderScene);

    prepareSphere(1.0f, 20, 20);

    glutMainLoop();
    return 0;
}*/