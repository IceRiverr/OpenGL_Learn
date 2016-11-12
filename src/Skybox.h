
#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include<GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>
#include <vector>
#include <string>

#include "Shader.h"
#include "Camera.h"


class Skybox
{
public:
	Skybox();
	~Skybox();
	
	void Init();
	void Draw(Camera* camera);

	void SetCubeMapFiles(const std::vector<std::string>& cubeMapFaces);

private:
	GLuint skyBoxVAO;
	GLuint skyBoxVBO;
	GLuint skyBoxMap;
	Shader* skyboxShader;
};

#endif // !_SKYBOX_H_
