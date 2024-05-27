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

    float deltaPhi = M_PI / stacks;
    float delta = 2.0f * M_PI / slices;

    // Bottom
    for (int i = 0; i < slices; i++) {
        float alfa = delta * i;

        // Center of the base
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);

        texCoord.push_back(0.5f);
        texCoord.push_back(0.5f);

        // First point on the circle
        vertices.push_back(radius * sin(alfa + delta));
        vertices.push_back(0.0f);
        vertices.push_back(radius * cos(alfa + delta));

        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);

        texCoord.push_back(0.5f + 0.5f * sin(alfa + delta));
        texCoord.push_back(0.5f + 0.5f * cos(alfa + delta));

        // Second point on the circle
        vertices.push_back(radius * sin(alfa));
        vertices.push_back(0.0f);
        vertices.push_back(radius * cos(alfa));

        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);

        texCoord.push_back(0.5f + 0.5f * sin(alfa));
        texCoord.push_back(0.5f + 0.5f * cos(alfa));
    }

    // Body
    for (int i = 0; i < stacks; i++) {
        float y = i * height / stacks;
        float next_y = (i + 1) * height / stacks;
        float r = radius * (1 - static_cast<float>(i) / stacks);
        float next_r = radius * (1 - static_cast<float>(i + 1) / stacks);

        for (int j = 0; j < slices; j++) {
            float alfa = delta * j;
            float nextAlfa = (j + 1) * delta;

            // First triangle
            vertices.push_back(r * sin(alfa));
            vertices.push_back(y);
            vertices.push_back(r * cos(alfa));

            normals.push_back(sin(alfa));
            normals.push_back(radius / height);
            normals.push_back(cos(alfa));

            texCoord.push_back(static_cast<float>(j) / slices);
            texCoord.push_back(static_cast<float>(i) / stacks);

            vertices.push_back(r * sin(nextAlfa));
            vertices.push_back(y);
            vertices.push_back(r * cos(nextAlfa));

            normals.push_back(sin(nextAlfa));
            normals.push_back(radius / height);
            normals.push_back(cos(nextAlfa));

            texCoord.push_back(static_cast<float>(j + 1) / slices);
            texCoord.push_back(static_cast<float>(i) / stacks);

            vertices.push_back(next_r * sin(alfa));
            vertices.push_back(next_y);
            vertices.push_back(next_r * cos(alfa));

            normals.push_back(sin(alfa));
            normals.push_back(radius / height);
            normals.push_back(cos(alfa));

            texCoord.push_back(static_cast<float>(j) / slices);
            texCoord.push_back(static_cast<float>(i + 1) / stacks);

            // Second triangle
            vertices.push_back(next_r * sin(alfa));
            vertices.push_back(next_y);
            vertices.push_back(next_r * cos(alfa));

            normals.push_back(sin(alfa));
            normals.push_back(radius / height);
            normals.push_back(cos(alfa));

            texCoord.push_back(static_cast<float>(j) / slices);
            texCoord.push_back(static_cast<float>(i + 1) / stacks);

            vertices.push_back(r * sin(nextAlfa));
            vertices.push_back(y);
            vertices.push_back(r * cos(nextAlfa));

            normals.push_back(sin(nextAlfa));
            normals.push_back(radius / height);
            normals.push_back(cos(nextAlfa));

            texCoord.push_back(static_cast<float>(j + 1) / slices);
            texCoord.push_back(static_cast<float>(i) / stacks);

            vertices.push_back(next_r * sin(nextAlfa));
            vertices.push_back(next_y);
            vertices.push_back(next_r * cos(nextAlfa));

            normals.push_back(sin(nextAlfa));
            normals.push_back(radius / height);
            normals.push_back(cos(nextAlfa));

            texCoord.push_back(static_cast<float>(j + 1) / slices);
            texCoord.push_back(static_cast<float>(i + 1) / stacks);
        }
    }

    modelData.vertices = vertices;
    modelData.normals = normals;
    modelData.texCoord = texCoord;

    return modelData;
}