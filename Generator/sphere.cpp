#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <vector>



ModelData sphere(float radius, int slices, int stacks) {

    ModelData modelData;

    //Vector creation
    std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoord;
    

    float deltaPhi = (float)(M_PI) / stacks;
    float deltaTheta = (float)(2 * M_PI) / slices;

    for (int i = 0; i < stacks; i++) {
        float phi = i * deltaPhi;
        float nextPhi = (i + 1) * deltaPhi;

        for (int j = 0; j < slices; j++) {
            float theta = j * deltaTheta;
            float nextTheta = (j + 1) * deltaTheta;

            // Primeiro triângulo do quadrilátero
            vertices.push_back(radius * sin(phi) * cos(theta));
            vertices.push_back(radius * cos(phi));
            vertices.push_back(radius * sin(phi) * sin(theta));

            normals.push_back(sin(phi) * cos(theta));
			normals.push_back(cos(phi));
			normals.push_back(sin(phi) * sin(theta));

            texCoord.push_back((float)j/slices);
            texCoord.push_back((float)i/stacks);


            vertices.push_back(radius * sin(phi) * cos(nextTheta));
            vertices.push_back(radius * cos(phi));
            vertices.push_back(radius * sin(phi) * sin(nextTheta));

            normals.push_back(sin(phi) * cos(nextTheta));
			normals.push_back(cos(phi));
			normals.push_back(sin(phi) * sin(nextTheta));

            texCoord.push_back((float)(j+1)/slices);
            texCoord.push_back((float)i/stacks);


            vertices.push_back(radius * sin(nextPhi) * cos(theta));
            vertices.push_back(radius * cos(nextPhi));
            vertices.push_back(radius * sin(nextPhi) * sin(theta));

            normals.push_back(sin(nextPhi) * cos(theta));
			normals.push_back(cos(nextPhi));
			normals.push_back(sin(nextPhi) * sin(theta));

            texCoord.push_back((float)j/slices);
            texCoord.push_back((float)(i+1)/stacks);


            // Segundo triângulo do quadrilátero
            vertices.push_back(radius * sin(nextPhi) * cos(theta));
            vertices.push_back(radius * cos(nextPhi));
            vertices.push_back(radius * sin(nextPhi) * sin(theta));

            normals.push_back(sin(nextPhi) * cos(theta));
			normals.push_back(cos(nextPhi));
			normals.push_back(sin(nextPhi) * sin(theta));

            texCoord.push_back((float)(j+1)/slices);
            texCoord.push_back((float)(i+1)/stacks);


            vertices.push_back(radius * sin(phi) * cos(nextTheta));
            vertices.push_back(radius * cos(phi));
            vertices.push_back(radius * sin(phi) * sin(nextTheta));

            normals.push_back(sin(phi) * cos(nextTheta));
			normals.push_back(cos(phi));
			normals.push_back(sin(phi) * sin(nextTheta));

            texCoord.push_back((float)j/slices);
            texCoord.push_back((float)(i+1)/stacks);


            vertices.push_back(radius * sin(nextPhi) * cos(nextTheta));
            vertices.push_back(radius * cos(nextPhi));
            vertices.push_back(radius * sin(nextPhi) * sin(nextTheta));

            normals.push_back(sin(nextPhi) * cos(nextTheta));
			normals.push_back(cos(nextPhi));
			normals.push_back(sin(nextPhi) * sin(nextTheta));

            texCoord.push_back((float)(j+1)/slices);
            texCoord.push_back((float)i/stacks);
        }
    }
    modelData.vertices = vertices;
	modelData.normals = normals;
	modelData.texCoord = texCoord;

	return modelData;
}