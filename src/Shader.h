#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class BasicShaderVSParams
{
public:
	void BindShader(GLuint& shaderProgram);
	void SetVSMatrix(const glm::mat4& modelM, const glm::mat4& viewM, const glm::mat4 projM);
	void SetModelMatrix(const glm::mat4& modelM);

private:
	GLint modelMLoc;
	GLint viewMLoc;
	GLint projMLoc;
};

class  Shader
{
public:
	Shader();
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();
	
	void Use();
	
	GLuint Program;
	BasicShaderVSParams vsParams;

private:
	void RemoveNotes(std::string& code);
};

#endif // !_SHADER_H_
