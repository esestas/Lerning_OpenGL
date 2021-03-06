#include "pch.h"
#include <iostream>
#include <algorithm>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "Image.h"
#include <TextureType.h>
#include <Soil/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Model.h>

using namespace glm;
using namespace std;

// Прототип функции
void key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode);
void do_movement();
void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);
void scroll_callback(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset);
void frameBuffer();
void drawScreenQuad();
void renderQuad();
void renderScene(Shader &shader);
void renderCube();
int f = 0;

void senMatrix(Shader s, mat4 m, mat4 v, mat4 p);

unsigned int* LoadTex(string texsPath[]);
unsigned int loadCubeMap(vector<string> faces);
string replaceChar(string str);

// Размер окна
const GLuint WIDTH = 1280, HEIGHT = 720;
//Цвет очистки буфера
const GLfloat clearColor[4] = { 1.0f, 0.5f, 0.5f, 1.0f };
// Вектора для камеры
vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);;
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);;
vec3 cameraUp;


//Источник света
static const vec3 LIGHT_POSITION = vec3(50.0f, 50.0f, -50.0f);
static const vec3 LIGHTCOLOR = vec3(1.0f, 1.0f, 1.0f);

Camera camera(glm::vec3(0.0f, 20.0f, 50.0f));
bool keys[1024];
GLfloat fov = glm::radians(60.0f);
//Поворот камеры
GLdouble lastX = WIDTH / 2;
GLdouble lastY = HEIGHT / 2;

GLfloat yaw1 = -90.0f;
GLfloat pitch1 = 0.0f;

GLboolean firstMouse = true;
//Время
GLfloat deltaTime = 0.0f;
GLdouble lastFrame = 0.0f;

unsigned int fbo;
unsigned int quadVAO;
unsigned int tex;

float RandomFloat(float a, float b) 
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
	cout << "Starting GLFW context, OpenGL 3.3" << endl;
	// Инициализация GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Ригестрируем функцию захвата кусора
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// Регестируем функцию mouse_callback
	glfwSetCursorPosCallback(window, mouse_callback);
	// Регестируем функцию scroll_callback
	glfwSetScrollCallback(window, scroll_callback);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) // Initialize GLEW to setup the OpenGL Function pointers
	{
		cout << "Failed to initialize GLEW" << endl;
		return -1;
	}

	// Define the viewport dimensions
	GLint width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	//glEnable(GL_MULTISAMPLE); // Enabled by default on some drivers, but not all so always enable to make sure
	glEnable(GL_DEPTH_TEST);

	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << endl;

	const char *pathV = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/UnlitShader_v.v";
	const char *pathF = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/UnlitShader_f.f";
	const char *pathFColor = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/ColorShader_f.f";

	//постпоцесинг
	const char *pathVScreen = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/ScreenShader.v";
	const char *pathFScreen = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/ScreenShader.f";

	//CubeMap
	const char *SkyBoxVScreen = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/SkyBoxShader.v";
	const char *SkyBoxFScreen = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/SkyBoxShader.f";

	//Reflect
	const char *reflectPathF = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/ReflectShader.f";
	const char *reflectPathV = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/ReflectShader.v";

	//Instansing
	const char *InstansingPathF = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/InstansingShader_f.f";
	const char *InstansingPathV = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/InstansingShader_v.v";

	//UBO
	const char *UBOPathV = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/UniformShader.v";
	const char *UBOPathF = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/UBOShader.f";

	//Blin-Fong 
	const char *BlinFongPathV = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/Blin-FongShader_v.v";
	const char *BlinFongPathF = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/Blin-FongShader_f.f";

	//Depth DepthShader_f.f
	const char *DepthPathV = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/DepthShader_v.v";
	const char *DepthPathF = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/DepthShader_f.f";

	//SimpleDepth 
	const char *SimpleDepthPathV = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/SimpleDepthShader_v.v";
	const char *SimpleDepthPathF = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/SimpleDepthShader_f.f";

	//QuadDepth 
	const char *DepthQuadPathV = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/DepthQuadShader.v";
	const char *DepthQuadPathF = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Shaders/DepthQuadShader.f";


	const char *pathFloorModel = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Models/NormalBox.obj";
	const char *pathBoxModel = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Models/newBox.obj";
	const char *pathWindowModel = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Models/Window.obj";
	const char *pathKnightModel = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Models/K.obj";

	//Textures
	const char *pathGroundTex = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Textures/Ground.png";
	const char *pathFloorTex = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Textures/Wood.png";
	const char *pathBoxTex = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Textures/test.png";
	const char *pathWindowTex = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Textures/window.png";
	const char *pathKnightColorTex = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Textures/Knight_Base_Color.jpg";
	const char *pathKnightMetalnessTex = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Textures/Knight_Metalness.jpg";
	const char *wallTex =       "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Textures/Blocks/BaseColor.png";
	const char *wallNormalTex = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Textures/Blocks/Normal.png";
	const char *GlossTex = "D:/Study/OPENGL/Project/LearnOpenGL/LearnOpenGL/Textures/Blocks/Gloss.png";

	Shader UnlitShader(pathV, pathF);
	Shader ColorShader(pathV, pathFColor);
	Shader ScreenShader(pathVScreen, pathFScreen);
	Shader SkyBoxShader(SkyBoxVScreen, SkyBoxFScreen);
	Shader ReflectShader(reflectPathV, reflectPathF);
	Shader UBOShader(UBOPathV, UBOPathF);
	Shader BlinFongShader(BlinFongPathV, BlinFongPathF);
	Shader DepthShader(DepthPathV, DepthPathF);
	Shader SimpleDepthShader(SimpleDepthPathV, SimpleDepthPathF);

	Shader InstansingShader(InstansingPathV, InstansingPathF);
	Shader DepthTest(SimpleDepthPathV, SimpleDepthPathF);
	Shader DebugDepthQuad(DepthQuadPathV, DepthQuadPathF);

	Model floorModel(pathFloorModel);
	Model boxModel(pathBoxModel);
	Model windowModel(pathWindowModel, pathWindowTex);
	Model KnightModel(pathKnightModel, pathKnightMetalnessTex);

	vec2 tile = vec2(2.0f, 2.0f);

	float moveSpeed = 20.0f;
	
	GLuint diffID;
	glGenTextures(1, &diffID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffID);

	//tilling parametrs 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//filtering paramtrs
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//int width, height;
	unsigned char* imageD;
	imageD = SOIL_load_image(wallTex, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageD);


	SOIL_free_image_data(imageD);

	GLuint normalID;
	glGenTextures(1, &normalID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalID);

	//tilling parametrs 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//filtering paramtrs
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	unsigned char* imageN;
	imageN = SOIL_load_image(wallNormalTex, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageN);

	SOIL_free_image_data(imageN);

	GLuint glossID;
	glGenTextures(1, &glossID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, glossID);

	//tilling parametrs 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//filtering paramtrs
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* imageG;
	imageG = SOIL_load_image(GlossTex, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageG);

	SOIL_free_image_data(imageG);
	glBindTexture(GL_TEXTURE_2D, 0);


	//Load textures
	//BlinFongShader.Use();
	//BlinFongShader.setInt("diffuseMap", diffID);
	//BlinFongShader.setInt("normalMap", normalID);
	glm::vec3 lightPos(30.0f, 50.0f, 0.0f);

	float near_plane = 0.1f, far_plane = 1000.0f;

	// Gameplay loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		// Расчет deltaTime
		GLdouble currentFrame = glfwGetTime() * moveSpeed;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//Перемещение камеры
		do_movement();
		
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glViewport(0, 0, WIDTH, HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//General matrix
		glm::mat4 view = mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0);

		view = camera.GetViewMatrix();
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, near_plane, far_plane);

		//float offsetY = sin(glfwGetTime() * 2.0f) * 32.0f;
		float offsetX = cos(glfwGetTime() * 2.0f) * 16.0f;

		//Visualise sun
		//model = glm::translate(model, vec3(lightPos.x + offsetX, lightPos.y + offsetY, lightPos.z));
		model = glm::translate(model, vec3(lightPos.x , lightPos.y, lightPos.z));
		model = glm::scale(model, glm::vec3(0.1f));
		senMatrix(ColorShader, model, view, projection);
		boxModel.Draw(ColorShader);

		//Draw floor
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -50.0));
		model = glm::rotate(model, glm::radians((float)(glfwGetTime() * 10.0f)), glm::normalize(glm::vec3(0.0, 1.0, .0)));
		model = glm::scale(model, glm::vec3(0.5f));


		if (keys[GLFW_KEY_Q])
		{
			f = 0;
		}

		if (keys[GLFW_KEY_E])
		{
			f = 1;
		}

		senMatrix(BlinFongShader, model, view, projection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffID);
		BlinFongShader.setInt("diffuseMap", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalID);
		BlinFongShader.setInt("normalMap", 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, glossID);
		BlinFongShader.setInt("glossMap", 2);
		BlinFongShader.setInt("f", f);
		//BlinFongShader.setVec3("ligthPos", lightPos.x + offsetX, lightPos.y + offsetY, lightPos.z);
		BlinFongShader.setVec3("ligthPos", lightPos.x , lightPos.y, lightPos.z);
		BlinFongShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		floorModel.Draw(BlinFongShader, tile);

		////Draw model
		//UnlitShader.Use();
		//model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		//model = glm::scale(model, glm::vec3(1.0f));
		//UnlitShader.setMat4("model", model);
		//view = camera.GetViewMatrix();
		//UnlitShader.setMat4("view", view);
		//projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f);
		//UnlitShader.setMat4("projection", projection);
		//KnightModel.Draw(UnlitShader);


		//Draw fullscreenquad
		//glDisable(GL_DEPTH_TEST);
		//glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		//glClear(GL_COLOR_BUFFER_BIT);

		//ScreenShader.Use();
		////glActiveTexture(GL_TEXTURE0);
		////glBindTexture(GL_TEXTURE_2D, depthMap);
		//ScreenShader.setInt("texTest", depthMap);
		//drawScreenQuad();
		
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

void senMatrix(Shader s, mat4 m, mat4 v, mat4 p)
{
	s.Use();
	s.setMat4("MODEL", m);
	v = camera.GetViewMatrix();
	s.setMat4("VIEW", v);
	s.setMat4("PROJECTION", p);
}

void renderScene(Shader &shader)
{
	// floor
	glm::mat4 model = glm::mat4(1.0f);
	//shader.setMat4("model", model);
	//glBindVertexArray(planeVAO);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.setMat4("model", model);
	renderCube();
}



void renderCube()
{
	unsigned int cubeVAO;
	unsigned int cubeVBO;
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

unsigned int loadCubeMap(vector<string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = SOIL_load_image(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
		}
		SOIL_free_image_data(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void drawScreenQuad()
{
	float quadVertices[] = 
	{ // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void frameBuffer()
{
	fbo; // ID фрейм буфера
	glGenFramebuffers(1, &fbo); // создали обьект буфера кадра
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); // привязали фрейм буфер

	// создаем пикрепление для кадрового буффера 
	// в текстуру либо в рендер буфер

	// используем текстурные прикрепления
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	// используем рендер буфер пикрипление

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "Frame buffer ok" << endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

string replaceChar(string str)
{
	const char n = '/';
	const char o = '(\)';
	std::replace(str.begin(), str.end(), o, n);
	return str;
}

void do_movement()
{
	if (keys[GLFW_KEY_W])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode)
{
	if (action == GLFW_PRESS)
		keys[key] = true;

	if (action == GLFW_RELEASE)
		keys[key] = false;
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLdouble xOffSet = xpos - lastX;
	GLdouble yOffSet = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xOffSet, yOffSet);
}

void scroll_callback(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}




void renderQuad()
{
	unsigned int quadVAO;
	unsigned int quadVBO;

	float quadVertices[] = 
	{
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	// setup plane VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}