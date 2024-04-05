#include <stdio.h>
#include "./include/parser.hpp"

const std::string MODEL_PATH = "../../modelFiles/";

std::vector<float> createModelsArray(std::vector<std::string> filenames){
	std::vector<float> vertices;

	cout << "Loading .3d files..." << "\n";

	for(int i = 0; i < filenames.size(); i++){

		std::string filename = filenames[i];

		std::ifstream file = std::ifstream(MODEL_PATH + filename);

		float coordinate;

		while(file >> coordinate){
			//cout << coordinate << "\n"; // DEBUG
			vertices.push_back(coordinate);
		}

		cout << filename << "\n";

		file.close();
	}

	cout << "Finished loading .3d files! " << vertices.size() << " vertices!" << "\n";

	return vertices;
}

void parseGroup (XMLElement* groupElem, Group& group) {

    XMLElement* transformElem = groupElem->FirstChildElement("transform");
    if (transformElem) {

        // translation
        XMLElement* translateElem = transformElem->FirstChildElement("translate");
        if(translateElem) {
            translateElem->QueryFloatAttribute("x", &group.transform.translate.x);
            translateElem->QueryFloatAttribute("y", &group.transform.translate.y);
            translateElem->QueryFloatAttribute("z", &group.transform.translate.z);
        }

        // rotation
        XMLElement* rotateElem = transformElem->FirstChildElement("rotate");
        if (rotateElem) {
            rotateElem->QueryFloatAttribute("angle", &group.transform.rotateAngle);
            rotateElem->QueryFloatAttribute("x", &group.transform.rotate.x);
            rotateElem->QueryFloatAttribute("y", &group.transform.rotate.y);
            rotateElem->QueryFloatAttribute("z", &group.transform.rotate.z);
        }

        // scale
        XMLElement* scaleElem = transformElem->FirstChildElement("scale");
        if (scaleElem) {
            scaleElem->QueryFloatAttribute("x", &group.transform.scale.x);
            scaleElem->QueryFloatAttribute("y", &group.transform.scale.y);
            scaleElem->QueryFloatAttribute("z", &group.transform.scale.z);
        } 
    }

    // model filenames
    std::vector<std::string> filenames;

    XMLElement* modelsElem = groupElem->FirstChildElement("models");
    if (modelsElem) {
        Model model;
        for (XMLElement* modelElem = modelsElem->FirstChildElement("model"); modelElem; modelElem = modelElem->NextSiblingElement("model")) {
            model.mod = modelElem->Attribute("file");
            group.models.push_back(model);
            filenames.push_back(model.mod);
        }
    }

    // model vertices
    group.groupVertices = createModelsArray(filenames);

    // recursive parsing groups
    for (XMLElement* childGroupElem = groupElem->FirstChildElement("group"); childGroupElem; childGroupElem = childGroupElem->NextSiblingElement("group")) {
        Group childGroup;
        parseGroup(childGroupElem, childGroup);
        group.children.push_back(childGroup);
    }
    
}

void parser (const char* file, World& world) {
    XMLDocument doc;

    if (doc.LoadFile(file) != XML_SUCCESS) {
        cerr << "Erro ao ler o ficheiro XML: " << file << endl;
        exit(1);
    }

    XMLElement* base = doc.FirstChildElement("world");
    if (!base) {
        cerr << "'World' nao foi encontrado no ficheiro XML." << endl;
        exit(1);
    }
    
    XMLElement* elem = base->FirstChildElement();
    while (elem) {

        //Window
        if (strcmp(elem->Name(), "window") == 0) {
             elem->QueryIntAttribute("width", &world.windowWidth);
             elem->QueryIntAttribute("height", &world.windowHeight);
        }

        //Camera
        if (strcmp(elem->Name(), "camera") == 0) {

            XMLElement* cameraElem = elem->FirstChildElement("position");       
            if (cameraElem) {
            cameraElem->QueryFloatAttribute("x", &world.camera.position.x);
            cameraElem->QueryFloatAttribute("y", &world.camera.position.y);
            cameraElem->QueryFloatAttribute("z", &world.camera.position.z);
            }

            cameraElem = elem->FirstChildElement("lookAt");       
            if (cameraElem) {
            cameraElem->QueryFloatAttribute("x", &world.camera.lookAt.x);
            cameraElem->QueryFloatAttribute("y", &world.camera.lookAt.y);
            cameraElem->QueryFloatAttribute("z", &world.camera.lookAt.z);
            }

            cameraElem = elem->FirstChildElement("up");       
            if (cameraElem) {
            cameraElem->QueryFloatAttribute("x", &world.camera.up.x);
            cameraElem->QueryFloatAttribute("y", &world.camera.up.y);
            cameraElem->QueryFloatAttribute("z", &world.camera.up.z);
            }

            cameraElem = elem->FirstChildElement("projection");       
            if (cameraElem) {
            cameraElem->QueryFloatAttribute("fov", &world.camera.projection.fov);
            cameraElem->QueryFloatAttribute("near", &world.camera.projection.near);
            cameraElem->QueryFloatAttribute("far", &world.camera.projection.far);
            }
        }

        elem = elem->NextSiblingElement();
    }

    // Groups
    parseGroup(base->FirstChildElement("group"), world.root);
}
