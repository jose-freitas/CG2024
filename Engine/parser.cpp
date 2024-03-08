#include <stdio.h>
#include "./include/parser.hpp"


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

        // Parsing models
        if (strcmp(elem->Name(), "group") == 0) {
            
            XMLElement* models = elem->FirstChildElement("models");
            
            if (models) {
                XMLElement* modElem;
                Model newModel;
                
                for (XMLElement* model = models->FirstChildElement("model"); model; model = model->NextSiblingElement("model")) {
                
                    //modElem = model->FirstChildElement();
                    newModel.mod = model->Attribute("file");
                
                    /*if (strcmp(model->Name(), "texture") == 0) {
                        newModel.texture = modElem->Attribute("texture");
                        modElem = modElem->NextSiblingElement();
                    }*/
                
                    world.models.push_back(newModel);


                }
            }
        }

        elem = elem->NextSiblingElement();
    }
}