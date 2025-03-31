#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include "stb_image.h"
#include "Shader.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Texture{
	public:
                GLuint ID = 0;
                GLuint unit;
        Texture() : ID(0), unit(0) {}
		void setTexture(const char* path,int e,GLuint unit){
            glGenTextures(1,&ID);
			glBindTexture(GL_TEXTURE_2D,ID);
                        this->unit = unit;
			//Set expansion mode
			if (e == 0)
        		{
           		 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        		}
       			else if (e == 1)
        		{
            			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        		}
        		else if (e == 2)
        		{
           	 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        		}
        		else if (e == 3)
       			{
            			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

            			float borderColor[] = { 0.0f,0.0f,0.0f,0.0f };
            			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        		}
			//Setting mipmaps
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//Loading image Texture
			int width, height, nrChannels;
			stbi_set_flip_vertically_on_load(true);
			unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
			 if (data)
        		{
            			GLenum format;
            			if (nrChannels == 1)
                			format = GL_RED;
            			else if (nrChannels == 3)
                			format = GL_RGB;
           			else if (nrChannels == 4)
                			format = GL_RGBA;

            			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            			glGenerateMipmap(GL_TEXTURE_2D);
        		}
        		else
       	 		{
            			cout << "ERROR::COULD NOT LOAD TEXTURE "<< path << endl;
        		}
        		stbi_image_free(data);
                }
                ~Texture(){}
                void texUnit(Shader& shader,const string& uniform,int u){
                        shader.setInt(uniform,u);
                }
		void BindTexture(Shader& program){
                        glActiveTexture(GL_TEXTURE0 + unit);
                        glBindTexture(GL_TEXTURE_2D,ID);
		}
        void Delete() {
            glDeleteTextures(1,&ID);
        }
};

#endif
