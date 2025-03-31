#ifndef MESH_H
#define MESH_H

#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <cmath>
#include "BO.h"
#include "Shader.h"
#include "Texture.h"

#define M_PI 3.14159265358979323846

using namespace std;

class Mesh{
	public:
		VAO vao;
		VBO vbo;
		EBO ebo;
		vector<Vertex>vertices;
		vector<unsigned int>indices;
		vector<Texture>textures;
		vec4 albedo = vec4(1.0f,0.0f,0.0f,1.0f); //Mesh Color
		bool visibleTexture = false;
		void setUpMesh(){
			vao.Generate();
			vbo.Generate();
			ebo.Generate();

			vao.Bind();
			vbo.MeshData(vertices);
			ebo.IndexData(indices);
			vao.AttribMesh(vertices);
			vao.Unbind();
			vbo.Unbind();
			ebo.Unbind();
		}
		void setUpPrimitive(vector<float>& data,vector<unsigned int>& faces){
			vao.Generate();
			vbo.Generate();
			ebo.Generate();
			vao.Bind();
			vbo.Data(data);
			ebo.IndexData(faces);
			vao.Attrib(0,3,8,0); //Positions
			vao.Attrib(1,2,8,3); //Texture Coordinates
			vao.Attrib(2,3,8,5); //Normals
			vao.Unbind();
			vbo.Unbind();
			ebo.Unbind();
		}
		Mesh(){}
		Mesh(vector<Vertex>& vertices,vector<unsigned int>& indices,vector<Texture>& textures){
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;
			setUpMesh();
		}
		void drawMesh(Shader& program){
			program.setBool("visibleTexture",visibleTexture);//Set flag to view texture
			program.setVec4("albedo",albedo);//Send mesh albedo color to shader
			if(textures.size()!=0 && visibleTexture){
				for(int i = 0; i < textures.size();i++){
					string uniform = "texture" + to_string(i);
					textures[i].texUnit(program,uniform.data(),i);
					textures[i].BindTexture(program);
				}
			}
			vao.Bind();
			glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
			vao.Unbind();
		}
		void Delete() {
			vao.Delete();
			ebo.Delete();
			vbo.Delete();
			for (int i = 0; i < textures.size(); i++) {
				textures[i].Delete();
			}
			textures.clear();
		}
};

//Mesh Primitives - this primitives are just for testing and debuggging
//Primitive buffer structure (Positions UV), so basically these primitives will only have position and texture
class Plane : public Mesh{
	public:
		vector<float>buffer={
			//Positions       //TexCoords //Normals
			-1.0f,0.0f,1.0f,   0.0f, 0.0f, 0.0f,1.0f,0.0f,
			 1.0f,0.0f,1.0f,   1.0f, 0.0f, 0.0f,1.0f,0.0f,
			-1.0f,0.0f,-1.0f,  0.0f, 1.0f, 0.0f,1.0f,0.0f,
			 1.0f,0.0f,-1.0f,  1.0f, 1.0f, 0.0f,1.0f,0.0f,
		};
		Plane(float x,float z, string texPath = "TEXTURES\\sampler.jpg"){
			//Set indices
			vector<unsigned int> faces = {0,1,2,1,2,3};
			indices = faces;
			//Define buffer data
			for(int i = 0; i < buffer.size();i += 8){
				buffer[i] *= x;
				buffer[i+2] *= z;
			}
			Texture texture;
			texture.setTexture(texPath.c_str(), 0, 0);
			textures.push_back(texture);
			textures.push_back(texture);
			setUpPrimitive(buffer,indices);
		}
};

class Cube : public Mesh{
	public:
		vector<float>buffer={
			//Position      TexCoords    Normals
			//Front
		       -1.0f,1.0f,1.0f,  0.0f, 0.0f, 0.0f,0.0f,1.0f,
		        1.0f,1.0f,1.0f,  1.0f, 0.0f, 0.0f,0.0f,1.0f,
		       -1.0f,-1.0f,1.0f, 0.0f, 1.0f, 0.0f,0.0f,1.0f,
		        1.0f,-1.0f,1.0f, 1.0f, 1.0f, 0.0f,0.0f,1.0f,
			//left
		       -1.0f,1.0f,-1.0f,  0.0f, 0.0f, -1.0f,0.0f,0.0f,
		       -1.0f,1.0f, 1.0f,  1.0f, 0.0f, -1.0f,0.0f,0.0f,
		       -1.0f,-1.0f,-1.0f, 0.0f, 1.0f, -1.0f,0.0f,0.0f,
		       -1.0f,-1.0f,1.0f,  1.0f, 1.0f, -1.0f,0.0f,0.0f,
		       //Right
		       1.0f,1.0f,-1.0f,   0.0f, 0.0f,  -1.0f,0.0f,0.0f,
		       1.0f,1.0f, 1.0f,  1.0f, 0.0f,   -1.0f,0.0f,0.0f,
		       1.0f,-1.0f,-1.0f, 0.0f, 1.0f,   -1.0f,0.0f,0.0f,
		       1.0f,-1.0f,1.0f,  1.0f, 1.0f,   -1.0f,0.0f,0.0f,
		       //Back
		       -1.0f,1.0f,-1.0f,  0.0f, 0.0f, 0.0f,0.0f,-1.0f,
		        1.0f,1.0f,-1.0f,  1.0f, 0.0f, 0.0f,0.0f,-1.0f,
		       -1.0f,-1.0f,-1.0f, 0.0f, 1.0f, 0.0f,0.0f,-1.0f,
		        1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 0.0f,0.0f,-1.0f,
			//Top
		       -1.0f,1.0f,-1.0f,  0.0f, 0.0f, 0.0f,1.0f,0.0f,
		        1.0f,1.0f,-1.0f,  1.0f, 0.0f, 0.0f,1.0f,0.0f,
		       -1.0f,1.0f,1.0f,  0.0f, 1.0f,  0.0f,1.0f,0.0f,
		        1.0f,1.0f,1.0f,   1.0f, 1.0f, 0.0f,1.0f,0.0f,
			//Bottom
		       -1.0f,-1.0f,-1.0f,  0.0f, 0.0f, 0.0f,-1.0f,0.0f,
		        1.0f,-1.0f,-1.0f,  1.0f, 0.0f, 0.0f,-1.0f,0.0f,
		       -1.0f,-1.0f,1.0f,   0.0f, 1.0f, 0.0f,-1.0f,0.0f,
		        1.0f,-1.0f,1.0f,   1.0f, 1.0f, 0.0f,-1.0f,0.0f,
		};
		Cube(float x, float y, float z,string texPath = "TEXTURES\\sampler.jpg") {
			vector<unsigned int>faces = {
				0,1,2,
				1,2,3,
				4,5,6,
				5,6,7,
				8,9,10,
				9,10,11,
				12,13,14,
				13,14,15,
				16,17,18,
				17,18,19,
				20,21,22,
				21,22,23,
			};
			for(int i = 0; i < buffer.size();i+=8){
				buffer[i] *= x;
				buffer[i+1] *= y;
				buffer[i+2] *= z;
			}
			indices = faces;
			Texture texture;
			texture.setTexture(texPath.c_str(), 0, 0);
			textures.push_back(texture);
			setUpPrimitive(buffer,indices);
		}
};

class UvSphere : public Mesh{
	public:
		vector<float>buffer;
		UvSphere(float radius,int latitudes,int longitudes, string texPath = "TEXTURES\\sampler.jpg"){ //Slices = longitudes, Stacks = latitudes
			float deltaLongitude = 2 * M_PI / longitudes; //Slice angle step
			float deltaLatitude = M_PI / latitudes; //Stack angle step
			float latitudeAngle; //phi
			float longitudeAngle; //theta
			float lengthInv = 1.0f / radius;
			if(longitudes < 3)
				longitudes = 3;
			if(latitudes < 2)
				latitudes = 2;

			for(int i = 0; i <= latitudes; ++i){
				latitudeAngle = M_PI / 2 - i * deltaLatitude; //Starting from -90 to 90
				float xy = radius * cos(latitudeAngle); //r * cos(phi)
				float z = radius * sin(latitudeAngle); //r * sin(phi)

				for(int j = 0; j <= longitudes; ++j){
					longitudeAngle = j * deltaLongitude;
					float x = xy * cos(longitudeAngle);  
					float y = xy * sin(longitudeAngle); 
					float s = (float)j/longitudes;
					float t = (float)i/latitudes;
					float nx = x * lengthInv;
					float ny = y * lengthInv;
					float nz = z * lengthInv;
					//Pushing z before y so it can be generated vertically
					buffer.push_back(x); buffer.push_back(z);buffer.push_back(y); buffer.push_back(s); buffer.push_back(t);
					buffer.push_back(nx); buffer.push_back(nz); buffer.push_back(ny);

				}
			}
			//Indices
			unsigned int k1, k2;
			 for(int i = 0; i < latitudes; ++i)
			{
				k1 = i * (longitudes + 1);
				k2 = k1 + longitudes + 1;
				// 2 Triangles per latitude block excluding the first and last longitudes blocks
				for(int j = 0; j < longitudes; ++j, ++k1, ++k2)
				{
					if (i != 0)
					{
						indices.push_back(k1);
						indices.push_back(k2);
						indices.push_back(k1 + 1);
					}

					if (i != (latitudes - 1))
					{
						indices.push_back(k1 + 1);
						indices.push_back(k2);
						indices.push_back(k2 + 1);
					}
				}
			}
			Texture texture;
			texture.setTexture(texPath.c_str(), 0, 0);
			textures.push_back(texture);
			setUpPrimitive(buffer,indices);
		}
};

class Capsule : public Mesh{
	public:
		vector<float>buffer;
		//Quick note, radius must be half or less the height of the capsule
		Capsule(float radius,float height,int latitudes,int longitudes,string texPath = "TEXTURES\\sampler.jpg"){ //Slices = longitudes, Stacks = latitudes
			float deltaLongitude = 2 * M_PI / longitudes; //Slice angle step
			float deltaLatitude = M_PI / latitudes; //Stack angle step
			float latitudeAngle; //phi
			float longitudeAngle; //theta
			if(radius >=  height/2)
				radius = height/2;
			float cylinderHeight = (height - 2.0f * radius);
			float lengthInv = 1.0f / radius;
			if(longitudes < 3)
				longitudes = 3;
			if(latitudes < 2)
				latitudes = 2;
			cout << "Cylinder Height: "<<cylinderHeight<<endl;
			for(int i = 0; i <= latitudes; ++i){
				latitudeAngle = M_PI / 2 - i * deltaLatitude; //Starting from -90 to 90
				float xy = radius * cos(latitudeAngle); //r * cos(phi)
				float z = radius * sin(latitudeAngle); //r * sin(phi)

				for(int j = 0; j <= longitudes; ++j){
					longitudeAngle = j * deltaLongitude;
					float x = xy * cos(longitudeAngle);  
					float y = xy * sin(longitudeAngle); 
					float s = (float)j/longitudes;
					float t = (float)i/latitudes;
					float nx = x * lengthInv;
					float ny = y * lengthInv;
					float nz = z * lengthInv;
					float zOffset = (z > 0.0f) ? (cylinderHeight / 2 + z) : (-cylinderHeight/2 + z);
					//float nzOffset = (z > 0.0f) ? (cylinderHeight / 2 + nz) : (-cylinderHeight/2 + nz);
					//Pushing z before y so it can be generated vertically
					buffer.push_back(x);
					buffer.push_back(zOffset);
					buffer.push_back(y); 
					buffer.push_back(s); 
					buffer.push_back(t);
					buffer.push_back(nx); buffer.push_back(nz); buffer.push_back(ny);
				}
			}
			//Indices
			unsigned int k1, k2;
			 for(int i = 0; i < latitudes; ++i)
			{
				k1 = i * (longitudes + 1);
				k2 = k1 + longitudes + 1;
				// 2 Triangles per latitude block excluding the first and last longitudes blocks
				for(int j = 0; j < longitudes; ++j, ++k1, ++k2)
				{
					if (i != 0)
					{
						indices.push_back(k1);
						indices.push_back(k2);
						indices.push_back(k1 + 1);
					}

					if (i != (latitudes - 1))
					{
						indices.push_back(k1 + 1);
						indices.push_back(k2);
						indices.push_back(k2 + 1);
					}
				}
			}
			Texture texture;
			texture.setTexture(texPath.c_str(), 0, 0);
			textures.push_back(texture);
			setUpPrimitive(buffer,indices);
		}
};
#endif
