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

    float *ToFloats () {
            float *floats = new float[4];
            floats[0] = x;
            floats[1] = y;
            floats[2] = z;
            floats[3] = 0.0f;
            return floats;
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

        float *ToFloats () {
            float *floats = new float[4];
            floats[0] = R / 255.0f;
            floats[1] = G / 255.0f;
            floats[2] = B / 255.0f;
            floats[3] = 1.0f;
            return floats;
        }
    } diffuse, ambient, specular, emissive;
    int shininess;
};

struct Texture {
    string file;
    GLuint texID;
};

struct Light {
    string type;
    Coords position;
    Coords direction;

    float cutoff;
};

struct ModelData {
    std::vector<float> modelVertices;
    std::vector<float> modelNormals;
    std::vector<float> modelUvs;
};

struct Model {
    string mod;
    Texture texture;
    Color color;
    GLuint renderVertices;
    GLuint renderNormals;
    GLuint renderUvs;
    ModelData modelData;
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
    vector<Light> lights;
};

void parser(const char* file, World& world);

#endif // PARSER_HPP