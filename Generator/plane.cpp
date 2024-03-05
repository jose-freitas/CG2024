#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <vector>

std::vector<float> plane (float dimension, int divisions) {
    //Vector creation
    std::vector<float> vertices;

    //Plane
    float unit = dimension / divisions;
	float half = dimension / 2.0f;

	for (int x = 0; x < divisions; x++){
		for (int z = 0; z < divisions; z++){
			// Left Triangle
			vertices.push_back(x * unit - half);
			vertices.push_back(0.0f);
			vertices.push_back(z* unit - half);

			vertices.push_back(x * unit - half);
			vertices.push_back(0.0f);
			vertices.push_back((z+1) * unit - half);

			vertices.push_back((x + 1) * unit - half);
			vertices.push_back(0.0f);
			vertices.push_back(z * unit - half);
			
			// Right Triangle
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

	return vertices;
}