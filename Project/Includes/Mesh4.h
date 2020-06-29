#ifndef CUSTOMMESH_H
#define CUSTOMMESH_H
#define GLEW_STATIC
#include <GL/glew.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include <TextureType.h>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<vector>
//using namespace std;

struct  Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	//glm::vec3 Bitangent;
};

class Mesh4
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	unsigned int VAO;
	GLuint tId;
	unsigned int VBO, EBO;

	Mesh4(vector<Vertex> vertices, vector<unsigned int> indices, const char *path)
	{
		this->vertices = vertices;
		this->indices = indices;
		setupMesh();
		LoadTex(path);
	}

	Mesh4(vector<Vertex> vertices, vector<unsigned int> indices)
	{
		this->vertices = vertices;
		this->indices = indices;
		setupMesh();
	}

	void Draw(Shader shader, vec2 tile)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tId);
		glUniform1i(glGetUniformLocation(shader.Program, "tex"), 0);
		GLint uvTileID = glGetUniformLocation(shader.Program, "tile");
		glUniform2f(uvTileID, (GLfloat)tile.x, (GLfloat)tile.y);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Draw(Shader shader, vec2 tile, unsigned int cubeID)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tId);
		glUniform1i(glGetUniformLocation(shader.Program, "tex"), 0);
		GLint uvTileID = glGetUniformLocation(shader.Program, "tile");
		glUniform2f(uvTileID, (GLfloat)tile.x, (GLfloat)tile.y);

		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeID);
		glUniform1i(glGetUniformLocation(shader.Program, "cubeMap"), 1);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void LoadTex(const char *path)
	{
		GLuint texiD;
		glGenTextures(1, &texiD);
		glBindTexture(GL_TEXTURE_2D, texiD);

		//tilling parametrs 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//filtering paramtrs
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height;
		
		string PNG = ".png";
		string sourse = path;
		unsigned char* image;
		if (sourse.find(PNG))
		{
			image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		}
		else
		{
			image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		}

		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
		this->tId = texiD;
	}

	void setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		// vertex position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		//// vertex bitangent
		//glEnableVertexAttribArray(4);
		//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

		glBindVertexArray(0);
	}
};

#endif // ! CUSTOMMESH_H