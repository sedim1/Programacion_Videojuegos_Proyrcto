#ifndef ENTITY_H
#define ENTITY_H

#include "Shader.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Todo tipo de entidad 3D visual que el ojo puede ver
class Entity3D{
public:
	Transform transform;
	Shader shader; //Shader con la cual la entidad que sera dibujado
	Entity3D(){}
	//Setters
	void setShader(Shader& program) {
		shader = program;
	}
	//Functions that will get call every frame
	void Draw(glm::mat4 view, glm::mat4 projection){}
	void Update(float deltaTime) {} //Procesar logica de la entidad
};

/*Entidades para el juego*/
class Player : public Entity3D {
public:
	Cube model = Cube(1.0f,1.0f,1.0f,"TEXTURES\\slime.png");
	Player(){
		model.visibleTexture = true;
	}
	~Player() {
		model.Delete();
	}
	
	void Draw(glm::mat4 view, glm::mat4 projection) {
		shader.use();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setMat4("model", transform.GetModelMatrix());
		model.drawMesh(shader);
	}

	void Update(){
	}
};

#endif
