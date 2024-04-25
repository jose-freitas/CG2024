#include <vector>
#include <cmath>

std::vector<float> torus(float inner_radius, float outer_radius, int slices, int stacks) {
    std::vector<float> vertices;
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

            vertices.push_back((outer_radius + inner_radius * cos(nextPhi)) * cos(theta)); // x2
            vertices.push_back(inner_radius * sin(nextPhi)); // y2
            vertices.push_back((outer_radius + inner_radius * cos(nextPhi)) * sin(theta)); // z2

            vertices.push_back((outer_radius + inner_radius * cos(phi)) * cos(nextTheta)); // x3
            vertices.push_back(inner_radius * sin(phi)); // y3
            vertices.push_back((outer_radius + inner_radius * cos(phi)) * sin(nextTheta)); // z3

            // Second triangle
            vertices.push_back((outer_radius + inner_radius * cos(nextPhi)) * cos(theta)); // x2
            vertices.push_back(inner_radius * sin(nextPhi)); // y2
            vertices.push_back((outer_radius + inner_radius * cos(nextPhi)) * sin(theta)); // z2

            vertices.push_back((outer_radius + inner_radius * cos(nextPhi)) * cos(nextTheta)); // x4
            vertices.push_back(inner_radius * sin(nextPhi)); // y4
            vertices.push_back((outer_radius + inner_radius * cos(nextPhi)) * sin(nextTheta)); // z4

            vertices.push_back((outer_radius + inner_radius * cos(phi)) * cos(nextTheta)); // x3
            vertices.push_back(inner_radius * sin(phi)); // y3
            vertices.push_back((outer_radius + inner_radius * cos(phi)) * sin(nextTheta)); // z3
        }
    }
    return vertices;
}
