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

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <vector>

void prepareCone(float radius, float height, int slices, int stacks) {

    std::vector<float> vertices;
    float delta = (2*M_PI) / slices;
    

    //Bottom
    for(int i = 0; i < slices; i++)
    {
        float alfa = delta * i;

		vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

		vertices.push_back(radius * sin(alfa + delta));
        vertices.push_back(0.0f);
        vertices.push_back(radius * cos(alfa + delta));
        
        vertices.push_back(radius * sin(alfa));
        vertices.push_back(0.0f);
        vertices.push_back(radius * cos(alfa));
    }


    //Body
    for(int i = 0; i<stacks; i++)
    {
        float y = i * height / stacks;
        float next_y = (i + 1) * height / stacks;
        float r = radius * (1 - static_cast<float>(i) / stacks);
        float next_r = radius * (1 - static_cast<float>(i + 1) / stacks);
        
        for(int j = 0; j<slices; j++)
        {
            float alfa = delta * j;

            vertices.push_back(r * sin(alfa));
            vertices.push_back(y);
            vertices.push_back(r * cos(alfa));

            vertices.push_back(r * sin(alfa + delta));
            vertices.push_back(y);
            vertices.push_back(r * cos(alfa + delta));

            vertices.push_back((next_r * sin(alfa)));
            vertices.push_back(next_y);
            vertices.push_back(next_r * cos(alfa));

            vertices.push_back(next_r * sin(alfa));
            vertices.push_back(next_y);
            vertices.push_back(next_r * cos(alfa));

            vertices.push_back(r * sin(alfa + delta));
            vertices.push_back(y);
            vertices.push_back(r * cos(alfa + delta));

            vertices.push_back(next_r * sin(alfa + delta));
            vertices.push_back(next_y);
            vertices.push_back(next_r * cos(alfa + delta));
        }
    }
}

