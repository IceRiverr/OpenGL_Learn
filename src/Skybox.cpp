
#include "Skybox.h"
#include "Tookit.h"

GLfloat skyboxVertices[] = {
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

Skybox::Skybox()
{

}
Skybox::~Skybox()
{
	glDeleteBuffers(1, &skyBoxVBO);
	glDeleteVertexArrays(1, &skyBoxVAO);
	glDeleteTextures(1, &skyBoxMap);
	delete skyboxShader; skyboxShader = nullptr;
}

void Skybox::Init()
{
	skyboxShader = new Shader("..\\shaders\\SkyBox_vs.glsl", "..\\shaders\\SkyBox_ps.glsl");
	
	glGenBuffers(1, &skyBoxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &skyBoxVAO);
	glBindVertexArray(skyBoxVAO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Skybox::Draw(Camera* camera)
{
	skyboxShader->Use();
	glBindVertexArray(skyBoxVAO);

	glm::mat4 modelM;

	glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewM()));

	skyboxShader->vsParams.SetVSMatrix(modelM, view, camera->GetPerspProjM());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxMap);
	glUniform1i(glGetUniformLocation(skyboxShader->Program, "cubeMap"), 0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);
}

void Skybox::SetCubeMapFiles(const std::vector<std::string>& cubeMapFaces)
{
	skyBoxMap = LoadCubeMap(cubeMapFaces);
}
