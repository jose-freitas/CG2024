#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector>
#include "../utils/tinyxml2.cpp"

using namespace tinyxml2;
using namespace std;

struct Coords {
    float x, y, z;
};

struct Transform {
    Coords translate;
    Coords rotate;
    Coords scale;
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
    Transform transform;
    struct Color {
        struct RGB {
            int R, G, B;
        } diffuse, ambient, specular, emissive;
        int shininess;
    } color;
};

struct Group {
    Transform transform; 
    vector<Model> models;
    vector<Group> children;
};

struct World {
    int windowWidth;
    int windowHeight;
    Camera camera;
    Group root;
};

void parser(const char* file, World& world);

#endif // PARSER_HPP