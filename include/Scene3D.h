#ifndef SCENE3D_H
#define SCENE3D_H

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Entity.h"
#include "Camera.h"

using namespace std;
using namespace glm;

//Base class
class Scene3D {
public:
	Camera viewportCamera;
	Camera gameCamera; 
	vector<Entity3D*>Entities; //Entidades3D que la escena va a contener
	bool enableViewport = true;
	Scene3D() {}
	Scene3D(Camera& viewport, Camera& ingame){
		this->viewportCamera = viewport;
		this->gameCamera = ingame;
	}
	Scene3D(Camera viewport, Camera ingame, vector<Entity3D*>&Entities) {
		this->viewportCamera = viewport;
		this->gameCamera = ingame;
		this->Entities = Entities;
	}
	~Scene3D() {
		for (Entity3D* entity : Entities) {
			delete entity;
		}
		Entities.clear();
	}
	void addObjectToScene(Entity3D* object){
		Entities.push_back(object);
	}
	void processEntities(float deltaTime){//Llamara a Update de todas las entidades para procesar la logica de4 cada uno respectivamente
		for (int i = 0; i < Entities.size();i++) {
			if (Entities[i]) {
				Entities[i]->Update(deltaTime);
			}
		}
	}
	//Mandara a llamar todas las entidades existentes a nuestra escena
	void drawEntities() {
		//Check wich camera is enable to draw with
		for (int i = 0; i < Entities.size(); i++) {
			Entities[i]->shader.use();
			if (enableViewport) {
				Entities[i]->Draw(viewportCamera.GetViewMatrix(),viewportCamera.GetProjectionMatrix());
			}
			else {
				Entities[i]->Draw(gameCamera.GetViewMatrix(),gameCamera.GetProjectionMatrix());
			}
		}
	}
	void ProcessScene(float deltaTime) {
		processEntities(deltaTime);
		drawEntities();
	}

	void viewportInput(GLFWwindow* window, float deltaTime) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			viewportCamera.ProcessKeyboard(FORWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			viewportCamera.ProcessKeyboard(BACKWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			viewportCamera.ProcessKeyboard(LEFT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			viewportCamera.ProcessKeyboard(RIGHT, deltaTime);
		}
	}
};

#endif