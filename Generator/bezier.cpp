#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <vector>


#include <fstream>
#include <string>
#include <iostream>



void cross(float* a, float* b, float* res) {

	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

// Função que multiplica uma matriz por um vetor
void multMatrixVector(float* m, float* v, float* res) {
	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}
}

void getBezierPoint(float u, float v, float (*points)[3], float* pos, float* du, float* dv) {
	float m[4][4] = { {-1.0f, 3.0f, -3.0f, 1.0f},
					  {3.0f, -6.0f, 3.0f, 0.0f},
					  {-3.0f, 3.0f, 0.0f, 0.0f},
					  {1.0f,  0.0f, 0.0f, 0.0f} };

	// Vetores U e V
	float U[4] = { u * u * u, u * u, u, 1 };
	float V[4] = { v * v * v, v * v, v, 1 };

    float U_linha[4] = { 3 * u * u, 2 * u, 1, 0 };
	float V_linha[4] = { 3 * v * v, 2 * v, 1, 0 };


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
		float A1_linha[4];
		float A2_linha[4];

		// Compute A1 = U * M
		multMatrixVector((float*)m, U, A1);

		// Compute A2 = A1 * P 
		multMatrixVector(p[i], A1, A2);


		// Compute A1 = A2 * MT (where MT = M)
		multMatrixVector((float*)m, A2, A1);

		// Mesma coisa mas com as derivadas para calcular as normais
		multMatrixVector((float*)m, U_linha, A1_linha);
		multMatrixVector(p[i], A1_linha, A2_linha);
		multMatrixVector((float*)m, A2_linha, A1_linha);

		// Compute pos[i] = A1 * V  (i.e. U * M * P * MT * V)
		pos[i] = V[0] * A1[0] + V[1] * A1[1] + V[2] * A1[2] + V[3] * A1[3];
		du[i] = V[0] * A1_linha[0] + V[1] * A1_linha[1] + V[2] * A1_linha[2] + V[3] * A1_linha[3];
		dv[i] = V_linha[0] * A1[0] + V_linha[1] * A1[1] + V_linha[2] * A1[2] + V_linha[3] * A1[3];
	}
}


ModelData bezier(const std::string& input_file,int tesselation)
{
    ModelData modelData;

    //Vector creation
    std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoord;

    std::ifstream patch_file(input_file);
    if (!patch_file) {
        std::cerr << "Erro ao abrir o arquivo: " << input_file << std::endl;
        return modelData;
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

    float pos[3], du[3], dv[3], normal[3];
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
                getBezierPoint(u, v, points, pos, du, dv);
                cross(du, dv, normal);
                vertices.push_back(pos[0]);
                vertices.push_back(pos[1]);
                vertices.push_back(pos[2]);
                normals.push_back(normal[0]);
                normals.push_back(normal[1]);
                normals.push_back(normal[2]);
                texCoord.push_back(u);
                texCoord.push_back(v);
                getBezierPoint(u, v + delta_v, points, pos, du, dv);
                cross(du, dv, normal);
                vertices.push_back(pos[0]);
                vertices.push_back(pos[1]);
                vertices.push_back(pos[2]);
                normals.push_back(normal[0]);
                normals.push_back(normal[1]);
                normals.push_back(normal[2]);
                texCoord.push_back(u + delta_u);
                texCoord.push_back(v);
                getBezierPoint(u + delta_u, v, points, pos, du, dv);
                cross(du, dv, normal);
                vertices.push_back(pos[0]);
                vertices.push_back(pos[1]);
                vertices.push_back(pos[2]);
                normals.push_back(normal[0]);
                normals.push_back(normal[1]);
                normals.push_back(normal[2]);
                texCoord.push_back(u);
                texCoord.push_back(v + delta_v);

                // Triangulo 2
                getBezierPoint(u + delta_u, v + delta_v, points, pos, du, dv);
                cross(du, dv, normal);
                vertices.push_back(pos[0]);
                vertices.push_back(pos[1]);
                vertices.push_back(pos[2]);
                normals.push_back(normal[0]);
                normals.push_back(normal[1]);
                normals.push_back(normal[2]);
                texCoord.push_back(u + delta_u);
                texCoord.push_back(v);
                getBezierPoint(u + delta_u, v, points, pos, du, dv);
                cross(du, dv, normal);
                vertices.push_back(pos[0]);
                vertices.push_back(pos[1]);
                vertices.push_back(pos[2]);
                normals.push_back(normal[0]);
                normals.push_back(normal[1]);
                normals.push_back(normal[2]);
                texCoord.push_back(u + delta_u);
                texCoord.push_back(v + delta_v);
                getBezierPoint(u, v + delta_v, points, pos, du, dv);
                cross(du, dv, normal);
                vertices.push_back(pos[0]);
                vertices.push_back(pos[1]);
                vertices.push_back(pos[2]);
                normals.push_back(normal[0]);
                normals.push_back(normal[1]);
                normals.push_back(normal[2]);
                texCoord.push_back(u);
                texCoord.push_back(v + delta_v);
            }
        }
    }

    modelData.vertices = vertices;
	modelData.normals = normals;
	modelData.texCoord = texCoord;

	return modelData;
}
