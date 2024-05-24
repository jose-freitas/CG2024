#ifndef PARSER_HPP
#define PARSER_HPP

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

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

struct Translate {
    float time;
    bool align;
    vector<Coords> points;
};

struct Rotate {
    float angle;
    float time;
    Coords point;
};

struct Transform {
    Coords currentTranslate;
    Translate translate;
    Rotate rotate;
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

struct Color {
    struct RGB {
        int R, G, B;
    } diffuse, ambient, specular, emissive;
    int shininess;
};

struct Texture {
    string file;
};

struct Light {
    string type;
    Coords position;
    Coords direction;
    float cutoff;
};

struct Model {
    string mod;
    Texture texture;
    Color color;
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
    vector<Light> lights;
};

void parser(const char* file, World& world);

#endif // PARSER_HPP