#ifndef SCENE3D_H
#define SCENE3D_H

#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>
#include "Entity.h"
#include "Camera.h"

using namespace std;

class Scene3D {
public:
	Camera viewportCamera;
	Camera gameCamera; 
	vector<Entity3D>Entities; //Entidades3D que la escena va a contener
	bool enableViewport = true;
	Scene3D() {}
	Scene3D(Camera viewport, Camera ingame){
		this->viewportCamera = viewport;
		this->gameCamera = ingame;
	}
	Scene3D(Camera viewport, Camera ingame, vector<Entity3D>Entities) {
		this->viewportCamera = viewport;
		this->gameCamera = ingame;
		this->Entities = Entities;
	}
	void addObjectToScene(Entity3D object){
		Entities.push_back(object);
	}
	void processEntities(float deltaTime){//Llamara a Update de todas las entidades para procesar la logica de4 cada uno respectivamente
		for (int i = 0; i < Entities.size();i++) {
			Entities[i].Update(deltaTime);
		}
	}
	//Mandara a llamar todas las entidades existentes a nuestra escena
	void drawEntities() {
		//Check wich camera is enable to draw with
		for (int i = 0; i < Entities.size(); i++) {
			if (enableViewport) {
				Entities[i].Draw(viewportCamera.GetViewMatrix(),viewportCamera.GetProjectionMatrix());
			}
			else {
				Entities[i].Draw(gameCamera.GetViewMatrix(),gameCamera.GetProjectionMatrix());
			}
		}
	}
};

#endif