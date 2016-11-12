
#include "Postprocess.h"

GLfloat fullscreenQuadVertices[] =
{
	-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
	1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 0.0f
};

GLuint fullscreenQuadIndices[6] =
{
	0, 1, 3,
	1, 2 ,3
};

Postprocess::~Postprocess()
{
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &quadEBO);
	glDeleteVertexArrays(1, &quadVAO);
	delete postprocessShader; postprocessShader = nullptr;
}

void Postprocess::Init()
{
	postprocessShader = new Shader("..\\shaders\\ScreenSpaceShader_vs.glsl", "..\\shaders\\PostProcess_ps.glsl");

	glGenBuffers(1, &quadVBO);
	glGenBuffers(1, &quadEBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullscreenQuadVertices), &fullscreenQuadVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(fullscreenQuadIndices), &fullscreenQuadIndices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Postprocess::Draw(GLuint colorTarget)
{
	postprocessShader->Use();

	glBindVertexArray(quadVAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTarget);
	glUniform1i(glGetUniformLocation(postprocessShader->Program, "mainMap"), 0);
	glm::mat4 modelM;
	GLint modelMLoc = glGetUniformLocation(postprocessShader->Program, "modelM");
	glUniformMatrix4fv(modelMLoc, 1, GL_FALSE, glm::value_ptr(modelM));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}