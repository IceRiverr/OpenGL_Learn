
#include "Light.h"
#include "Camera.h"

void Light::BindShape(BoxShape* light)
{
	pLightShape = light;
}

glm::vec3 GetLightPosition(GLuint index)
{
	static glm::vec3 pointLightPositions[] = {
		glm::vec3(4.0f,  4.0f,  2.0f),
		glm::vec3(-4.0f, 4.0f, 2.0f),
		glm::vec3(4.0f,  -4.0f, -3.0f),
		glm::vec3(-4.0f,  -4.0f, -3.0f)
	};

	if (index >= 0 && index < LIGNT_COUNT)
		return pointLightPositions[index];
	else
		return glm::vec3();
}

void BindLightShaderParams(GLuint shaderProgram, Camera* camera)
{
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.lightDir"), 1.0f, -1.0f, -1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
	// Point light 1
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].lightPos"), GetLightPosition(0).x, GetLightPosition(0).y, GetLightPosition(0).z);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].linear"), 0.09);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].quadratic"), 0.032);
	// Point light 2
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].lightPos"), GetLightPosition(1).x, GetLightPosition(1).y, GetLightPosition(1).z);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[1].linear"), 0.09);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[1].quadratic"), 0.032);
	// Point light 3
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].lightPos"), GetLightPosition(2).x, GetLightPosition(2).y, GetLightPosition(2).z);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[2].linear"), 0.09);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[2].quadratic"), 0.032);
	// Point light 4
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].lightPos"), GetLightPosition(3).x, GetLightPosition(3).y, GetLightPosition(3).z);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[3].linear"), 0.09);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[3].quadratic"), 0.032);
	//// SpotLight
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.lightPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.lightDir"), camera->cameraFront.x, camera->cameraFront.y, camera->cameraFront.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.linear"), 0.09);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.quadratic"), 0.032);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.cutoff"), glm::cos(glm::radians(15.0f)));
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.outerCutoff"), glm::cos(glm::radians(30.0f)));
}