
#ifndef _TOOKIT_H_
#define _TOOKIT_H_

#include<GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>
#include <vector>
#include <string>

GLuint LoadTextureFromFile(const char* fileName);
GLuint LoadCubeMap(const std::vector<std::string>& cubeMapFaces);

#endif