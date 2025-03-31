#ifndef ENTITY_H
#define ENTITY_H

#include "Shader.h"
#include "Transform.h"
#include <iostream>
#include "Mesh.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Todo tipo de entidad 3D visual que el ojo puede ver
class Entity3D{
public:
	Transform transform;
	Shader shader; //Shader con la cual la entidad que sera dibujado
	Mesh mesh;
	Entity3D() {}
	Entity3D(Shader shader){
		this->shader = shader;
	}
	//Setters
	void setShader(Shader program) {
		shader = program;
	}
	//Functions that will get call every frame
	void Draw(glm::mat4 view, glm::mat4 projection){
		shader.use();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setMat4("model", transform.GetModelMatrix());
		mesh.drawMesh(shader);
	}
	virtual void Update(float deltaTime) {} //Procesar logica de la entidad
};

/*Entidades para el juego*/
class Player : public Entity3D {
public:
	Player() {
		mesh = Cube(1.0f, 1.0f, 1.0f, "TEXTURES\\slime.png");
		mesh.visibleTexture = true;
	}
	void Draw(glm::mat4 view, glm::mat4 projection) {
		std::cout << "DrawingPlayer" << std::endl;
		shader.use();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setMat4("model", transform.GetModelMatrix());
		mesh.drawMesh(shader);
	}
	void Update(float deltaTime) override {
	}
};

class MapPlane : public Entity3D {
public:

	MapPlane() {
		mesh = Plane(10.0f, 10.0f);
		transform.translation.y = -1.2f;
		transform.translation.z = -2.5f;
		mesh.visibleTexture = true;
	}
	void Update(float deltaTime) override {
	}
};
#endif
