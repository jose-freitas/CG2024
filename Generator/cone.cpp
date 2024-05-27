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


ModelData cone(float radius, float height, int slices, int stacks) {
    ModelData modelData;

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoord;


    float deltaPhi = (float)(M_PI) / stacks;
    float delta = (2.0f * M_PI) / slices;
    

    //Bottom
    for(int i = 0; i < slices; i++)
    {
        float alfa = delta * i;

		vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);

        texCoord.push_back((float) (i+1) /slices);
        texCoord.push_back(0.0f);


		vertices.push_back(radius * sin(alfa + delta));
        vertices.push_back(0.0f);
        vertices.push_back(radius * cos(alfa + delta));

        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);

        texCoord.push_back((float)i /slices);
        texCoord.push_back(0.0f);

        
        vertices.push_back(radius * sin(alfa));
        vertices.push_back(0.0f);
        vertices.push_back(radius * cos(alfa));

        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);

        texCoord.push_back(0.5f);
        texCoord.push_back(0.5f);
    }


    //Body
    for(int i = 0; i<stacks; i++)
    {
        float phi = i * deltaPhi;
        float nextPhi = (i + 1) * deltaPhi;

        float y = i * height / stacks;
        float next_y = (i + 1) * height / stacks;
        float r = radius * (1 - static_cast<float>(i) / stacks);
        float next_r = radius * (1 - static_cast<float>(i + 1) / stacks);
        
        for(int j = 0; j<slices; j++)
        {
            float alfa = delta * j;
            float nextAlfa = (j + 1) * delta;

            vertices.push_back(r * sin(alfa));
            vertices.push_back(y);
            vertices.push_back(r * cos(alfa));

            normals.push_back(sin(j * ((2 * M_PI) / slices)));
			normals.push_back(height/stacks);
			normals.push_back(cos(j * ((2 * M_PI) / slices)));

            texCoord.push_back((float)j/slices);
            texCoord.push_back((float)i/stacks);


            vertices.push_back(r * sin(alfa + delta));
            vertices.push_back(y);
            vertices.push_back(r * cos(alfa + delta));

            normals.push_back(sin((j + 1) * ((2 * M_PI) / slices)));
			normals.push_back(height/stacks);
			normals.push_back(cos((j + 1) * ((2 * M_PI) / slices)));

            texCoord.push_back((float)(j+1) / slices);
            texCoord.push_back((float)i / stacks);


            vertices.push_back((next_r * sin(alfa)));
            vertices.push_back(next_y);
            vertices.push_back(next_r * cos(alfa));

            normals.push_back(sin(j * ((2 * M_PI) / slices)));
			normals.push_back(height/stacks);
			normals.push_back(cos(j * ((2 * M_PI) / slices)));

            texCoord.push_back((float)j / slices);
            texCoord.push_back((float)(i+1) / stacks);


            vertices.push_back(next_r * sin(alfa));
            vertices.push_back(next_y);
            vertices.push_back(next_r * cos(alfa));

            normals.push_back(sin(j * ((2 * M_PI) / slices)));
			normals.push_back(height / stacks);
			normals.push_back(cos(j * ((2 * M_PI) / slices)));

            texCoord.push_back((float)j / slices);
            texCoord.push_back((float)(i+1) / stacks);


            vertices.push_back(r * sin(alfa + delta));
            vertices.push_back(y);
            vertices.push_back(r * cos(alfa + delta));

            normals.push_back(sin((j + 1) * ((2 * M_PI) / slices)));
			normals.push_back(height / stacks);
			normals.push_back(cos((j + 1) * ((2 * M_PI) / slices)));

            texCoord.push_back((float)(j+1) / slices);
            texCoord.push_back((float)i / stacks);


            vertices.push_back(next_r * sin(alfa + delta));
            vertices.push_back(next_y);
            vertices.push_back(next_r * cos(alfa + delta));

            normals.push_back(sin((j + 1) * ((2 * M_PI) / slices)));
			normals.push_back(height / stacks);
			normals.push_back(cos((j + 1) * ((2 * M_PI) / slices)));

            texCoord.push_back((float)(j+1) / slices);
            texCoord.push_back((float)(i+1) / stacks);
        }
    }
    modelData.vertices = vertices;
	modelData.normals = normals;
	modelData.texCoord = texCoord;

	return modelData;
}