#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"

using namespace std;
using namespace Assimp;
using namespace glm;


class Model{
	public:
		Model() {}
		Model(const string& path){
			loadModel(path);
		}
		void Draw(Shader& shader){
			shader.use();
			for(int i = 0;i < meshes.size();i++)
				meshes[i].drawMesh(shader);
		}
	private:
		vector<Mesh> meshes;
		string directory;
		void loadModel(const string& path){
			//Load the model file
			Importer importer;
			const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
			if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE | !scene->mRootNode){
				cout << "ERROR::COULD NOT LOAD " << path << endl;
				return;
			}
			directory = path.substr(0,path.find_last_of('\\'));
			processNode(scene->mRootNode,scene);
			cout << "MODEL LOADED SUCCESFULLY:: " << directory << endl;
		}
		void processNode(aiNode* node,const aiScene* scene){
			//Process all node meshes
			for(unsigned int i = 0; i < node->mNumMeshes; i++){
				aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh,scene));
			}
			//Do the same for its children
			for(unsigned int i = 0; i < node->mNumChildren;i++)
				processNode(node->mChildren[i],scene);
		}
		Mesh processMesh(aiMesh* mesh,const aiScene* scene){
			vector<Vertex>vertices;
			vector<unsigned int>indices;
			vector<Texture> textures;
			//Process vertex data
			for(unsigned int i = 0; i < mesh->mNumVertices;i++){
				Vertex vertex;
				//Process positions
				vertex.Position = vec3(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z);

				//Process normals
				vertex.Normal = vec3(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z);
				//Process Texture Coordinates
				if(mesh->mTextureCoords[0])
					vertex.TexCoord = vec2(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y);
				else
					vertex.TexCoord = vec2(0.0f,0.0f);
				vertices.push_back(vertex);
			}
			//Process indices
			for(unsigned int i = 0; i < mesh->mNumFaces;i++){
				aiFace face = mesh->mFaces[i];
				for(unsigned int j = 0; j < face.mNumIndices;j++)
					indices.push_back(face.mIndices[j]);
			}

			//Process materials
			if(mesh->mMaterialIndex>=0){
				aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
				textures = loadTextures(material);
			}
			return Mesh(vertices,indices,textures);
		}
		vector<Texture> loadTextures(aiMaterial* mat){
			vector<Texture> textures;
			//load diffuse
			for(unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE);i++){
				aiString str;
				mat->GetTexture(aiTextureType_DIFFUSE,i,&str);//Get texture file location
				string texturePath = directory + "\\" + str.C_Str();
				cout<<"LOADING TEXTURE... "<<texturePath<<endl;
				//textures.createTexture(texturePath.data(),0);
				//textures.push_back(Texture(texturePath.data(),0,i));
				Texture tex = Texture();
				tex.setTexture(texturePath.c_str(),0,i);
				textures.push_back(tex);
			}
			//cout<<"Amount of textures this mesh has: "<<textures.totalTextures()<<endl;
			return textures;
		}
};

#endif
