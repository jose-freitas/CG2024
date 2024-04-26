#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <vector>


#include <fstream>
#include <string>
#include <iostream>

// Função que multiplica uma matriz por um vetor
void multMatrixVector(float* m, float* v, float* res) {
	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}
}

void getBezierPoint(float u, float v, float (*points)[3], float* pos) {
	float m[4][4] = { {-1.0f, 3.0f, -3.0f, 1.0f},
					  {3.0f, -6.0f, 3.0f, 0.0f},
					  {-3.0f, 3.0f, 0.0f, 0.0f},
					  {1.0f,  0.0f, 0.0f, 0.0f} };

	// Vetores U e V
	float U[4] = { u * u * u, u * u, u, 1 };
	float V[4] = { v * v * v, v * v, v, 1 };

	// Armazena cada componente dos 16 pontos
	float p[3][16];
	for (int i = 0; i < 16; i++) {
		p[0][i] = points[i][0];
		p[1][i] = points[i][1];
		p[2][i] = points[i][2];
	}

	// Cálculo para cada componente x, y e z do ponto
	for (int i = 0; i < 3; i++) { // x, y, z
		float A1[4];
		float A2[4];

		// Compute A1 = U * M
		multMatrixVector((float*)m, U, A1);

		// Compute A2 = A1 * P 
		multMatrixVector(p[i], A1, A2);

		// Compute A1 = A2 * MT (where MT = M)
		multMatrixVector((float*)m, A2, A1);

		// Compute pos[i] = A1 * V  (i.e. U * M * P * MT * V)
		pos[i] = V[0] * A1[0] + V[1] * A1[1] + V[2] * A1[2] + V[3] * A1[3];	
	}
}

std::vector<float> bezier(const std::string& input_file,int tesselation) {
    std::vector<float> vertices;

    std::ifstream patch_file(input_file);
    if (!patch_file) {
        std::cerr << "Erro ao abrir o arquivo: " << input_file << std::endl;
        return vertices;
    }

    int n_patches, n_cpoints;

    // Lê o número de patches
    patch_file >> n_patches;

    std::vector<std::vector<int>> patches(n_patches);
    for (int i = 0; i < n_patches; i++) {
        for (int j = 0; j < 16; j++) {
            std::string line;
			patch_file >> line;
            patches[i].push_back(stoi(line));
        }
    }

    // Lê o número de pontos de controle
    patch_file >> n_cpoints;

    std::vector<std::vector<float>> cpoints(n_cpoints);
    for (int i = 0; i < n_cpoints; i++) {
        for (int j = 0; j < 3; j++) {
            std::string line;
			patch_file >> line;
            cpoints[i].push_back(stof(line));
        }
    }

    patch_file.close();

    int vert = n_patches * 6 * tesselation * tesselation;

    float delta_u, delta_v;
    delta_u = delta_v = 1.0f / tesselation;

    float pos[3];
    for (int i = 0; i < patches.size(); i++) {
        float points[16][3];
        for (int k = 0; k < 16; k++) {
            int index = patches[i][k];
            for (int l = 0; l < 3; l++) {
                points[k][l] = cpoints[index][l];
            }
        }
        for (float u = 0; u < 1; u += delta_u) {
            for (float v = 0; v < 1; v += delta_v) {
                // Triangulo 1
                getBezierPoint(u, v, points, pos);
                vertices.push_back(pos[0]);
                vertices.push_back(pos[1]);
                vertices.push_back(pos[2]);
                getBezierPoint(u, v + delta_v, points, pos);
                vertices.push_back(pos[0]);
                vertices.push_back(pos[1]);
                vertices.push_back(pos[2]);
                getBezierPoint(u + delta_u, v, points, pos);
                vertices.push_back(pos[0]);
                vertices.push_back(pos[1]);
                vertices.push_back(pos[2]);

                // Triangulo 2
                getBezierPoint(u + delta_u, v + delta_v, points, pos);
                vertices.push_back(pos[0]);
                vertices.push_back(pos[1]);
                vertices.push_back(pos[2]);
                getBezierPoint(u + delta_u, v, points, pos);
                vertices.push_back(pos[0]);
                vertices.push_back(pos[1]);
                vertices.push_back(pos[2]);
                getBezierPoint(u, v + delta_v, points, pos);
                vertices.push_back(pos[0]);
                vertices.push_back(pos[1]);
                vertices.push_back(pos[2]);
            }
        }
    }

    return vertices;
}
