#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;
using namespace std;

class Shader
{
public:
	// ID  програграммы
	GLuint Program;

	// В конструкторе считываем и собираем шейдер
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		// Путь нашего шейдера
		string vertexCode, fragmentCode;
		//Переменые для чтения файлов
		ifstream vShaderFile, fShaderFile;

		vShaderFile.exceptions(ifstream::failbit);
		fShaderFile.exceptions(ifstream::failbit);

		try
		{
			//Открываем файлы с шейдерами
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			stringstream vShaderStream, fShaderStream;

			//Считываем данные в потоки
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//Закрываем файлы
			vShaderFile.close();
			fShaderFile.close();
			// Преобразовываем потоки  в массив GLchar
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (ifstream::failure e)
		{
			cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
		}

		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();

		//Сборка шейдера
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		};

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		};

		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);

		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
		}
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	};

	// Используем шейдер
	void Use()
	{
		glUseProgram(this->Program);
	};

	void setInt(const GLchar* param, GLint val)
	{
		GLint valID = glGetUniformLocation(this->Program, param);
		glUniform1i(valID, val);
	}

	void setFloat(const GLchar* param, GLfloat val)
	{
		GLint valID = glGetUniformLocation(this->Program, param);
		glUniform1f(valID, val);
	};

	void setVec2(const GLchar* param, GLfloat valX, GLfloat valY)
	{
		GLint valID = glGetUniformLocation(this->Program, param);
		glUniform2f(valID, valX, valY);
	}

	void setVec3(const GLchar* param, GLfloat valX, GLfloat valY, GLfloat valZ)
	{
		GLint valID = glGetUniformLocation(this->Program, param);
		glUniform3f(valID, valX, valY, valZ);
	}

	void setMat4(const GLchar* name, const glm::mat4 &mat)
	{
		GLint valID = glGetUniformLocation(this->Program, name);
		glUniformMatrix4fv(valID, 1, GL_FALSE, &mat[0][0]);
	}
};

#endif // !SHADER_H