#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <vector>

void plane (float dimension, int divisions) {
    int i;

    //criação do vector
    std::vector<float> vertices;

    //plano
    float unit = dimension / divisions;
	float half = dimension / 2.0f;

	for (int x = 0; x < divisions; x++){
		for (int z = 0; z < divisions; z++){
			// Triangle 1
			vertices.push_back(x * unit - half);
			vertices.push_back(0.0f);
			vertices.push_back(z* unit - half);

			vertices.push_back(x * unit - half);
			vertices.push_back(0.0f);
			vertices.push_back((z+1) * unit - half);

			vertices.push_back((x + 1) * unit - half);
			vertices.push_back(0.0f);
			vertices.push_back(z * unit - half);
			
			// Triangle 2
			vertices.push_back((x + 1) * unit - half);
			vertices.push_back(0.0f);
			vertices.push_back(z * unit - half);

			vertices.push_back(x * unit - half);
			vertices.push_back(0.0f);
			vertices.push_back((z+1) * unit - half);

			vertices.push_back((x+1) * unit - half);
			vertices.push_back(0.0f);
			vertices.push_back((z+1) * unit - half);
		}
	}


}