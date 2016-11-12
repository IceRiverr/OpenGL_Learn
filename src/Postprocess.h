
#ifndef _POSTPROCESS_H_
#define _POSTPROCESS_H_

#include<GL\glew.h>
#include <GLFW\glfw3.h>

#include "Shader.h"

class Postprocess
{
public:
	~Postprocess();
	
	void Init();
	void Draw(GLuint colorTarget);

private:
	GLuint quadVAO;
	GLuint quadVBO;
	GLuint quadEBO;

	Shader* postprocessShader;
};

#endif // !_POSTPROCESS_H_
