#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform{
	public:
		glm::vec3 translation;
		glm::vec3 rotation;
		glm::vec3 scaling;
		Transform(){
			translation = glm::vec3(0.0f,0.0f,0.0f);
			rotation = glm::vec3(0.0f,0.0f,0.0f);
			scaling = glm::vec3(1.0f,1.0f,1.0f);
		}
		glm::mat4 GetModelMatrix(){
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model,translation);
			model = glm::rotate(model,glm::radians(rotation.x),glm::vec3(1.0f,0.0f,0.0f));
			model = glm::rotate(model,glm::radians(rotation.y),glm::vec3(0.0f,1.0f,0.0f));
			model = glm::rotate(model,glm::radians(rotation.z),glm::vec3(0.0f,0.0f,1.0f));
			model = glm::scale(model,scaling);
			return model;
		}
};

#endif
