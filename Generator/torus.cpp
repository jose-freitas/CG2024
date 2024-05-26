#include <vector>
#include <cmath>




ModelData torus(float inner_radius, float outer_radius, int slices, int stacks) {

    ModelData modelData;

    //Vector creation
    std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoord;


    float majorDelta = (2 * M_PI) / slices;
    float minorDelta = (2 * M_PI) / stacks;

    for (int i = 0; i < slices; i++) {
        float theta = majorDelta * i;
        float nextTheta = majorDelta * (i + 1);

        for (int j = 0; j < stacks; j++) {
            float phi = minorDelta * j;
            float nextPhi = minorDelta * (j + 1);

            // First triangle
            vertices.push_back((outer_radius + inner_radius * cos(phi)) * cos(theta)); // x1
            vertices.push_back(inner_radius * sin(phi)); // y1
            vertices.push_back((outer_radius + inner_radius * cos(phi)) * sin(theta)); // z1

            normals.push_back(0.0f); 
            normals.push_back(0.1f); 
            normals.push_back(0.0f); 

            texCoord.push_back((float)i / slices);
            texCoord.push_back((float)j / stacks);


            vertices.push_back((outer_radius + inner_radius * cos(nextPhi)) * cos(theta)); // x2
            vertices.push_back(inner_radius * sin(nextPhi)); // y2
            vertices.push_back((outer_radius + inner_radius * cos(nextPhi)) * sin(theta)); // z2

            normals.push_back(0.0f); 
            normals.push_back(0.1f); 
            normals.push_back(0.0f); 

            texCoord.push_back((float)(i+1) / slices);
            texCoord.push_back((float)j / stacks);


            vertices.push_back((outer_radius + inner_radius * cos(phi)) * cos(nextTheta)); // x3
            vertices.push_back(inner_radius * sin(phi)); // y3
            vertices.push_back((outer_radius + inner_radius * cos(phi)) * sin(nextTheta)); // z3

            normals.push_back(0.0f); 
            normals.push_back(0.1f); 
            normals.push_back(0.0f); 

            texCoord.push_back((float)(i+1) / slices);
            texCoord.push_back((float)j / stacks);

            // Second triangle
            vertices.push_back((outer_radius + inner_radius * cos(nextPhi)) * cos(theta)); // x2
            vertices.push_back(inner_radius * sin(nextPhi)); // y2
            vertices.push_back((outer_radius + inner_radius * cos(nextPhi)) * sin(theta)); // z2

            normals.push_back(0.0f); 
            normals.push_back(0.1f); 
            normals.push_back(0.0f); 

            texCoord.push_back((float)(i+1) / slices);
            texCoord.push_back((float)j / stacks);


            vertices.push_back((outer_radius + inner_radius * cos(nextPhi)) * cos(nextTheta)); // x4
            vertices.push_back(inner_radius * sin(nextPhi)); // y4
            vertices.push_back((outer_radius + inner_radius * cos(nextPhi)) * sin(nextTheta)); // z4

            normals.push_back(0.0f); 
            normals.push_back(0.1f); 
            normals.push_back(0.0f); 

            texCoord.push_back((float)i / slices);
            texCoord.push_back((float)j / stacks);


            vertices.push_back((outer_radius + inner_radius * cos(phi)) * cos(nextTheta)); // x3
            vertices.push_back(inner_radius * sin(phi)); // y3
            vertices.push_back((outer_radius + inner_radius * cos(phi)) * sin(nextTheta)); // z3

            normals.push_back(0.0f); 
            normals.push_back(0.1f); 
            normals.push_back(0.0f); 

            texCoord.push_back((float)i / slices);
            texCoord.push_back((float)j / stacks);
        }
    }

    modelData.vertices = vertices;
	modelData.normals = normals;
	modelData.texCoord = texCoord;

	return modelData;
}
