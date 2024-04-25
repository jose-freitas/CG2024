#include "plane.cpp"
#include "box.cpp"
#include "cone.cpp"
#include "sphere.cpp"
#include "bezier.cpp"

#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>


const std::string MODEL_PATH = "../../modelFiles/";

void writeFile (std::vector<float> vertices, std::string filename){

    std::ofstream file = std::ofstream(MODEL_PATH+filename);

    for(int i = 0; i < vertices.size(); i++){
        file << vertices[i];

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

        std::vector<float> vertices = plane(dimension, divisions);

        writeFile(vertices, filename);
    }
    else if("box" == type){
        float dimension = std::stof(argv[2]);
        int divisions = std::stoi(argv[3]);
        std::string filename = std::string(argv[4]);

        std::vector<float> vertices = box(dimension, divisions);

        writeFile(vertices, filename);
    }
    else if("cone" == type){
        float radius = std::stof(argv[2]);
        float height = std::stof(argv[3]);
        int slices = std::stoi(argv[4]);
        int stacks = std::stoi(argv[5]);
        std::string filename = std::string(argv[6]);

        std::vector<float> vertices = cone(radius, height, slices, stacks);

        writeFile(vertices, filename);
    }
    else if("sphere" == type){
        float radius = std::stof(argv[2]);
        int slices = std::stoi(argv[3]);
        int stacks = std::stoi(argv[4]);
        std::string filename = std::string(argv[5]);

        std::vector<float> vertices = sphere(radius, slices, stacks);

        writeFile(vertices, filename);
    }
    else if("bezier" == type){
        std::string input_file = argv[2]; 
        int tesselation = std::stoi(argv[3]); 
        std::string filename = std::string(argv[4]); 

        std::vector<float> vertices = bezier(input_file, tesselation);

        writeFile(vertices, filename);
    } 
    else if("torus" == type){

        float inner_radius = atof(argv[2]);
		float outer_radius = atof(argv[3]);
		int slices = atoi(argv[4]);
		int stacks = atoi(argv[5]);
        std::string filename = std::string(argv[6]); 

        std::vector<float> vertices = torus(inner_radius, outer_radius, slices, stacks, filename);

        writeFile(vertices, filename);
    } 
    else {
        std::cout << "No model type selected!\n";
    }
}


