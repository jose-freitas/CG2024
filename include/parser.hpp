#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector>
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;

struct Coords {
    float x, y, z;
};

struct Camera {
    Coords position;
    Coords lookAt;
    Coords up;
    struct Projection {
        float fov, near, far;
    } projection;
};

struct Model {
    string mod;
    string texture;
    struct Color {
        struct RGB {
            int R, G, B;
        } diffuse, ambient, specular, emissive;
        int shininess;
    } color;
};

struct World {
    int windowWidth;
    int windowHeight;
    Camera camera;
    vector<Model> models;
};

void parser(const char* file, World& world);

#endif // PARSER_HPP