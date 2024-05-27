#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>



ModelData plane (float dimension, int divisions) {

	ModelData modelData;

    //Vector creation
    std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoord;

    //Plane
    float unit = dimension / divisions;
	float half = dimension / 2.0f;

	for (int x = 0; x < divisions; x++){
		for (int z = 0; z < divisions; z++){
			// Left Triangle
			vertices.push_back(x * unit - half);
			vertices.push_back(0.0f);
			vertices.push_back(z* unit - half);

			normals.push_back(0.0f);
			normals.push_back(1.0f);
			normals.push_back(0.0f);

			texCoord.push_back((x * unit) / dimension);
			texCoord.push_back((z * unit) / dimension);


			vertices.push_back(x * unit - half);
			vertices.push_back(0.0f);
			vertices.push_back((z+1) * unit - half);

			normals.push_back(0.0f);
			normals.push_back(1.0f);
			normals.push_back(0.0f);

			texCoord.push_back((x * unit) / dimension);
			texCoord.push_back(((z+1) * unit) / dimension);


			vertices.push_back((x + 1) * unit - half);
			vertices.push_back(0.0f);
			vertices.push_back(z * unit - half);

			normals.push_back(0.0f);
			normals.push_back(1.0f);
			normals.push_back(0.0f);

			texCoord.push_back(((x+1) * unit) / dimension);
			texCoord.push_back((z * unit) / dimension);

			
			// Right Triangle
			vertices.push_back((x + 1) * unit - half);
			vertices.push_back(0.0f);
			vertices.push_back(z * unit - half);

			normals.push_back(0.0f);
			normals.push_back(1.0f);
			normals.push_back(0.0f);

			texCoord.push_back(((x+1) * unit) / dimension);
			texCoord.push_back((z * unit) / dimension);


			vertices.push_back(x * unit - half);
			vertices.push_back(0.0f);
			vertices.push_back((z+1) * unit - half);

			normals.push_back(0.0f);
			normals.push_back(1.0f);
			normals.push_back(0.0f);

			texCoord.push_back((x * unit) / dimension);
			texCoord.push_back(((z+1) * unit) / dimension);


			vertices.push_back((x+1) * unit - half);
			vertices.push_back(0.0f);
			vertices.push_back((z+1) * unit - half);

			normals.push_back(0.0f);
			normals.push_back(1.0f);
			normals.push_back(0.0f);

			texCoord.push_back(((x+1) * unit) / dimension);
			texCoord.push_back(((z+1) * unit) / dimension);
		}
	}

	modelData.vertices = vertices;
	modelData.normals = normals;
	modelData.texCoord = texCoord;

	return modelData;
}