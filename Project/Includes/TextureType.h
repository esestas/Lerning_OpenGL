#ifndef TEXTURE_TYPE
#define TEXTURE_TYPE

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<vector>

enum TextureType
{
	tex1D = GL_TEXTURE_1D,
	tex2D = GL_TEXTURE_2D,
	tex3D = GL_TEXTURE_3D,
	Cube = GL_TEXTURE_CUBE_MAP
};

enum  RepeatMode
{
	clamp = GL_CLAMP,
	repeat = GL_REPEAT
};

struct Texture
{
	GLuint id; //?
	const char *path;
	const char *name;
	GLuint type;
	GLuint repeatMode;
};

#endif
