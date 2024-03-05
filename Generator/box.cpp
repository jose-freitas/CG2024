#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <vector>

std::vector<float> box (float dimension, int divisions) {
    //criação do vector
    std::vector<float> vertices;

    float unit = dimension / divisions;
	float half = dimension / 2.0f;

    // Top
	for (int x = 0; x < divisions; x++){
		for (int z = 0; z < divisions; z++){
			// Left Triangle
			vertices.push_back(x * unit - half);
			vertices.push_back(half);
			vertices.push_back(z* unit - half);

			vertices.push_back(x * unit - half);
			vertices.push_back(half);
			vertices.push_back((z+1) * unit - half);

			vertices.push_back((x + 1) * unit - half);
			vertices.push_back(half);
			vertices.push_back(z * unit - half);
			
			// Right Triangle
			vertices.push_back((x + 1) * unit - half);
			vertices.push_back(half);
			vertices.push_back(z * unit - half);

			vertices.push_back(x * unit - half);
			vertices.push_back(half);
			vertices.push_back((z+1) * unit - half);

			vertices.push_back((x+1) * unit - half);
			vertices.push_back(half);
			vertices.push_back((z+1) * unit - half);
		}
	}

    //Bottom
    for (int x = 0; x < divisions; x++){
		for (int z = 0; z < divisions; z++){
			// Left Triangle
            vertices.push_back((x + 1) * unit - half);
			vertices.push_back(-half);
			vertices.push_back(z * unit - half);

			vertices.push_back(x * unit - half);
			vertices.push_back(-half);
			vertices.push_back((z+1) * unit - half);

			vertices.push_back(x * unit - half);
			vertices.push_back(-half);
			vertices.push_back(z* unit - half);
			
			// Right Triangle
            vertices.push_back((x+1) * unit - half);
			vertices.push_back(-half);
			vertices.push_back((z+1) * unit - half);

			vertices.push_back(x * unit - half);
			vertices.push_back(-half);
			vertices.push_back((z+1) * unit - half);

			vertices.push_back((x + 1) * unit - half);
			vertices.push_back(-half);
			vertices.push_back(z * unit - half);
		}
	}

	//Back
    for (int x = 0; x < divisions; x++){
		for (int z = 0; z < divisions; z++){
			// Left Triangle
			vertices.push_back((x + 1) * unit - half);
			vertices.push_back(half - z * unit);
			vertices.push_back(- half);

			vertices.push_back(x * unit - half);
			vertices.push_back(half - (z+1) * unit);
			vertices.push_back(- half);

			vertices.push_back(x * unit - half);
			vertices.push_back(half - z * unit);
			vertices.push_back(- half);
			
			// Right Triangle
			vertices.push_back((x + 1) * unit - half);
			vertices.push_back(half - z * unit);
			vertices.push_back(- half);

			vertices.push_back((x + 1) * unit - half);
			vertices.push_back(half - (z+1) * unit);
			vertices.push_back(- half);

			vertices.push_back(x * unit - half);
			vertices.push_back(half - (z+1) * unit);
			vertices.push_back(- half);
		}
	}

	//Front
	for (int x = 0; x < divisions; x++){
		for (int z = 0; z < divisions; z++){
			// Left Triangle
			vertices.push_back(x * unit - half);
			vertices.push_back(half - z * unit);
			vertices.push_back(half);

			vertices.push_back(x * unit - half);
			vertices.push_back(half - (z+1) * unit);
			vertices.push_back(half);

			vertices.push_back((x + 1) * unit - half);
			vertices.push_back(half - z * unit);
			vertices.push_back(half);
			
			// Right Triangle
			vertices.push_back(x * unit - half);
			vertices.push_back(half - (z+1) * unit);
			vertices.push_back(half);

			vertices.push_back((x + 1) * unit - half);
			vertices.push_back(half - (z+1) * unit);
			vertices.push_back(half);

			vertices.push_back((x + 1) * unit - half);
			vertices.push_back(half - z * unit);
			vertices.push_back(half);
		}
	}

	//Left
    for (int x = 0; x < divisions; x++){
		for (int z = 0; z < divisions; z++){
			// Left Triangle 
			vertices.push_back(- half);
			vertices.push_back(half - (x + 1) * unit);
			vertices.push_back(z * unit - half);

			vertices.push_back(- half);
			vertices.push_back(half - x * unit);
			vertices.push_back((z+1) * unit - half);

			vertices.push_back(- half);
			vertices.push_back(half - x * unit);
			vertices.push_back(z * unit - half);
			
			// Right Triangle
			vertices.push_back(- half);
			vertices.push_back(half - (x + 1) * unit);
			vertices.push_back(z * unit - half);

			vertices.push_back(- half);
			vertices.push_back(half - (x + 1) * unit);
			vertices.push_back((z+1) * unit - half);

			vertices.push_back(- half);
			vertices.push_back(half - x * unit);
			vertices.push_back((z + 1) * unit - half);
		}
	}

    //Right
    for (int x = 0; x < divisions; x++){
		for (int z = 0; z < divisions; z++){
			// Left Triangle 
            vertices.push_back(half);
			vertices.push_back(half - x * unit);
			vertices.push_back(z * unit - half);

			vertices.push_back(half);
			vertices.push_back(half - x * unit);
			vertices.push_back((z+1) * unit - half);

			vertices.push_back(half);
			vertices.push_back(half - (x + 1) * unit);
			vertices.push_back(z * unit - half);
			
			// Right Triangle
            vertices.push_back(half);
			vertices.push_back(half - x * unit);
			vertices.push_back((z + 1) * unit - half);

			vertices.push_back(half);
			vertices.push_back(half - (x + 1) * unit);
			vertices.push_back((z+1) * unit - half);

			vertices.push_back(half);
			vertices.push_back(half - (x + 1) * unit);
			vertices.push_back(z * unit - half);
		}
	}
	return vertices;
}