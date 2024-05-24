#include <stdio.h>
#include <iostream>
#include <fstream>
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

        if (translateElem) {
            if (translateElem->QueryFloatAttribute("time", &group.transform.translate.time) == XML_NO_ATTRIBUTE) group.transform.translate.time = 0.0f;
            translateElem->QueryBoolAttribute("align", &group.transform.translate.align);

        // Parse Catmull-Rom curve points
            for (XMLElement* pointElem = translateElem->FirstChildElement("point"); pointElem; pointElem = pointElem->NextSiblingElement("point")) {
                float x, y, z;
                pointElem->QueryFloatAttribute("x", &x);
                pointElem->QueryFloatAttribute("y", &y);
                pointElem->QueryFloatAttribute("z", &z);
                group.transform.translate.points.push_back(Coords{x, y, z});
            }

            group.transform.currentTranslate = group.transform.translate.points[0];
        }
        else {
            group.transform.translate.time = 0.0f;
            group.transform.translate.align = true;
            group.transform.translate.points.push_back(Coords{0, 0, 0});
            group.transform.translate.points.push_back(Coords{0, 0, 0});
            group.transform.translate.points.push_back(Coords{0, 0, 0});
            group.transform.translate.points.push_back(Coords{0, 0, 0});
            group.transform.currentTranslate = group.transform.translate.points[0];
        }
    

        // rotation
        XMLElement* rotateElem = transformElem->FirstChildElement("rotate");

        if (rotateElem) {
            if (rotateElem->QueryFloatAttribute("time", &group.transform.rotate.time) == XML_NO_ATTRIBUTE) group.transform.rotate.time = 0.0f;
            rotateElem->QueryFloatAttribute("angle", &group.transform.rotate.angle);
            rotateElem->QueryFloatAttribute("x", &group.transform.rotate.point.x);
            rotateElem->QueryFloatAttribute("y", &group.transform.rotate.point.y);
            rotateElem->QueryFloatAttribute("z", &group.transform.rotate.point.z);
        }
        else {
            group.transform.rotate.time = 0.0f;
            group.transform.rotate.point = Coords { 0.0f, 0.0f, 0.0f };
            group.transform.rotate.angle = 0.0f;
        }

        // scale
        XMLElement* scaleElem = transformElem->FirstChildElement("scale");

        if (scaleElem) 
        {
            scaleElem->QueryFloatAttribute("x", &group.transform.scale.x);
            scaleElem->QueryFloatAttribute("y", &group.transform.scale.y);
            scaleElem->QueryFloatAttribute("z", &group.transform.scale.z);

            // Force default Scale to always be (1, 1, 1)
            Coords groupScale = group.transform.scale;

            if (groupScale.x == 0.0f || groupScale.y == 0.0f || groupScale.z == 0.0f) {
                group.transform.scale = Coords { 1.0f, 1.0f, 1.0f };
            }
        } 
        else {
            group.transform.scale = Coords { 1.0f, 1.0f, 1.0f };
        }
    }
    else
    {
        group.transform.currentTranslate = Coords { 0.0f, 0.0f, 0.0f };
        group.transform.rotate.point = Coords { 0.0f, 0.0f, 0.0f };
        group.transform.rotate.angle = 0.0f;
        group.transform.scale = Coords { 1.0f, 1.0f, 1.0f };
    }

    // model filenames
    std::vector<std::string> filenames;

    XMLElement* modelsElem = groupElem->FirstChildElement("models");
    if (modelsElem) {
        Model model;
        for (XMLElement* modelElem = modelsElem->FirstChildElement("model"); modelElem; modelElem = modelElem->NextSiblingElement("model")) {
            model.mod = modelElem->Attribute("file");

            // Texture
            XMLElement* textureElem = modelElem->FirstChildElement("texture");
            if (textureElem) {
                model.texture.file = textureElem->Attribute("file");
            }

            // Color
            XMLElement* colorElem = modelElem->FirstChildElement("color");
            if (colorElem) {
                XMLElement* colorsubElem = colorElem->FirstChildElement("diffuse");
                if (colorsubElem) {
                    diffuseElem->QueryIntAttribute("R", &model.color.diffuse.R);
                    diffuseElem->QueryIntAttribute("G", &model.color.diffuse.G);
                    diffuseElem->QueryIntAttribute("B", &model.color.diffuse.B);
                }
                colorsubElem = colorElem->FirstChildElement("ambient");
                if (colorsubElem) {
                    ambientElem->QueryIntAttribute("R", &model.color.ambient.R);
                    ambientElem->QueryIntAttribute("G", &model.color.ambient.G);
                    ambientElem->QueryIntAttribute("B", &model.color.ambient.B);
                }
                colorsubElem = colorElem->FirstChildElement("specular");
                if (colorsubElem) {
                    specularElem->QueryIntAttribute("R", &model.color.specular.R);
                    specularElem->QueryIntAttribute("G", &model.color.specular.G);
                    specularElem->QueryIntAttribute("B", &model.color.specular.B);
                }
                colorsubelemElem = colorElem->FirstChildElement("emissive");
                if (colorsubElem) {
                    emissiveElem->QueryIntAttribute("R", &model.color.emissive.R);
                    emissiveElem->QueryIntAttribute("G", &model.color.emissive.G);
                    emissiveElem->QueryIntAttribute("B", &model.color.emissive.B);
                }
                colorsubelemElem = colorElem->FirstChildElement("shininess");
                if (colorsubElem) {
                    shininessElem->QueryIntAttribute("value", &model.color.shininess);
                }
            }

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

    // Lights
        if (strcmp(elem->Name(), "lights") == 0) {
            for (XMLElement* lightElem = elem->FirstChildElement("light"); lightElem; lightElem = lightElem->NextSiblingElement("light")) {
                Light light;
                light.type = lightElem->Attribute("type");
                if (light.type == "point" || light.type == "spotlight") {
                    lightElem->QueryFloatAttribute("posX", &light.position.x);
                    lightElem->QueryFloatAttribute("posY", &light.position.y);
                    lightElem->QueryFloatAttribute("posZ", &light.position.z);
                }
                if (light.type == "directional" || light.type == "spotlight") {
                    lightElem->QueryFloatAttribute("dirX", &light.direction.x);
                    lightElem->QueryFloatAttribute("dirY", &light.direction.y);
                    lightElem->QueryFloatAttribute("dirZ", &light.direction.z);
                }
                if (light.type == "spotlight") {
                    lightElem->QueryFloatAttribute("cutoff", &light.cutoff);
                }
                world.lights.push_back(light);
            }
        }

    // Groups
    parseGroup(base->FirstChildElement("group"), world.root);
}