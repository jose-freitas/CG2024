#ifndef PARSER_HPP
#define PARSER_HPP

#include <stdio.h>
#include <iostream>
#include <vector>
#include "../utils/tinyxml2.cpp"

using namespace tinyxml2;
using namespace std;

struct Coords {
    float x, y, z;

    Coords Add(Coords coords) {
        
        return Coords {
            x = x + coords.x,
            y = y + coords.y,
            z = z + coords.z
        };

    }

    Coords Mult(Coords coords) {
        
        return Coords {
            x = x * coords.x,
            y = y * coords.y,
            z = z * coords.z
        };
    }
};

struct Transform {
    Coords translate;
    Coords rotate;
    float rotateAngle;
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
    struct Color {
        struct RGB {
            int R, G, B;
        } diffuse, ambient, specular, emissive;
        int shininess;
    } color;
};

struct Group {
    Transform transform; 
    GLuint renderVertices;
    std::vector<float> groupVertices;
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