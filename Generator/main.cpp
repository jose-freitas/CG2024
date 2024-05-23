#include "plane.cpp"
#include "box.cpp"
#include "cone.cpp"
#include "sphere.cpp"
#include "bezier.cpp"
#include "torus.cpp"

#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>


const std::string MODEL_PATH = "../../modelFiles/";

void writeFile (std::vector<float> vertices, std::vector<float> normals, std::vector<float> texCoord, std::string filename){

    std::ofstream file = std::ofstream(MODEL_PATH+filename);

    for(int i = 0; i < vertices.size(); i++){
        file << vertices[i];

        if ((i + 1) % 9 == 0)
            file << "\n";
        else 
            file << " ";
    }

    for(int i = 0; i < normals.size(); i++){
        file << normals[i];

        if ((i + 1) % 9 == 0)
            file << "\n";
        else 
            file << " ";
    }

    for(int i = 0; i < texCoord.size(); i++){
        file << normals[i];

        if ((i + 1) % 9 == 0)
            file << "\n";
        else 
            file << " ";
    }


    std::cout << "Created file... " << (filename) << "\n";

    file.close();
}


int main(int argc, char **argv) {
    std::string type = argv[1];
    
    std::cout << "Building model...\n";

   std::cout << "Type = " << type << " ? " << ("plane" == type) << "\n";

    if("plane" == type) {
        float dimension = std::stof(argv[2]);
        int divisions = std::stoi(argv[3]);
        std::string filename = std::string(argv[4]);

        std::vector<float> vertices, normals, texCoord = plane(dimension, divisions);

        writeFile(vertices, normals, texCoord, filename);
    }
    else if("box" == type){
        float dimension = std::stof(argv[2]);
        int divisions = std::stoi(argv[3]);
        std::string filename = std::string(argv[4]);

        std::vector<float> vertices, normals, texCoord = box(dimension, divisions);

        writeFile(vertices, normals, texCoord, filename);
    }
    else if("cone" == type){
        float radius = std::stof(argv[2]);
        float height = std::stof(argv[3]);
        int slices = std::stoi(argv[4]);
        int stacks = std::stoi(argv[5]);
        std::string filename = std::string(argv[6]);

        std::vector<float> vertices, normals, texCoord = cone(radius, height, slices, stacks);

        writeFile(vertices, normals, texCoord, filename);
    }
    else if("sphere" == type){
        float radius = std::stof(argv[2]);
        int slices = std::stoi(argv[3]);
        int stacks = std::stoi(argv[4]);
        std::string filename = std::string(argv[5]);

        std::vector<float> vertices, normals, texCoord = sphere(radius, slices, stacks);

        writeFile(vertices, normals, texCoord, filename);
    }
    else if("patch" == type){
        std::string input_file = argv[2]; 
        int tesselation = std::stoi(argv[3]); 
        std::string filename = std::string(argv[4]); 

        std::vector<float> vertices, normals, texCoord = bezier(input_file, tesselation);

        writeFile(vertices, normals, texCoord, filename);
    } 
    else if("torus" == type){

        float inner_radius = atof(argv[2]);
		float outer_radius = atof(argv[3]);
		int slices = atoi(argv[4]);
		int stacks = atoi(argv[5]);
        std::string filename = std::string(argv[6]); 

        std::vector<float> vertices, normals, texCoord = torus(inner_radius, outer_radius, slices, stacks);

        writeFile(vertices, normals, texCoord, filename);
    } 
    else {
        std::cout << "No model type selected!\n";
    }
}


