#ifndef iMAGE_H
#define iMAGE_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <Soil/SOIL.h>
using namespace std;

class Image
{
	// ID программы
	public: GLuint textureID;

		Image (const GLchar* imagePath)
		{
			// Размер текстуры
			GLint texSize;

			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			//Параметры тайлинга
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// Параметры фильтрации
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//Читаем текстуры как массив байт
			unsigned char* image = SOIL_load_image(imagePath, &texSize, &texSize, 0, SOIL_LOAD_RGB);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSize, texSize, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);

			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0);
		};
};
#endif // !IMAGE_H