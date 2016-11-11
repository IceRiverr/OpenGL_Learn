
#include <stdio.h>
#include <iostream>

//#define GLEW_STATIC //error
#include<GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"
#include "Shape.h"
#include "MeshData.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Model.h"
#include "FrameBuffer.h"

Shader* g_pShader = nullptr;
Shader* g_pLightObjectShader = nullptr;
Shader* g_pSingleColorShader = nullptr;
Shader* g_pGrassShader = nullptr;
Shader* g_pLineShader = nullptr;
Shader* g_pScreensapceShader = nullptr;

Shape* g_pQuadShape = nullptr;
BoxShape* g_pBoxShape = nullptr;
Model* g_pModel = nullptr;

Camera* g_pCamera = nullptr;
GLFWwindow* g_pWindow;
Light* g_pLight;
FrameBuffer* g_pFrameBuffer;

GLint g_nWidth, g_nHeight;
GLuint g_RockTex;
GLuint g_ArrowTex;
GLuint g_WordTex;
GLuint g_GrassTex;
GLuint g_GlassTex;

GLuint g_ContainerD;
GLuint g_ContainerS;
GLuint g_ContainerE;

GLuint quadVAO;
GLuint quadVBO;
GLuint quadEBO;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode); 
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void InitResource();
void Update(GLfloat dt);
void Render();
void Destroy();

void DrawGlass();
void DrawLight();
void PostProcess();

void CreateTexture(GLuint& tex, const char* fileName);
void Ohter_Test();
void BindLightShaderParams(GLuint shaderProgram);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	g_pWindow = glfwCreateWindow(1200, 900, "OpenGL", nullptr, nullptr);
	if (g_pWindow == nullptr)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(g_pWindow);
	glfwSetKeyCallback(g_pWindow, key_callback);
	glfwSetCursorPosCallback(g_pWindow, mouse_callback);
	glfwSetScrollCallback(g_pWindow,scroll_callback);
	glfwSetInputMode(g_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //GLFW_CURSOR_HIDDEN GLFW_CURSOR_DISABLED

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to init GLEW." << std::endl;
		return -1;
 	}

	glfwGetFramebufferSize(g_pWindow, &g_nWidth, &g_nHeight);
	glViewport(0, 0, g_nWidth, g_nHeight);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);// Ïß¿òÄ£Ê½
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InitResource();
	Ohter_Test();

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	
	GLfloat deltaTime = 0.0f;
	GLdouble lastFrame = 0.0f;

	while (!glfwWindowShouldClose(g_pWindow))
	{
		GLdouble currentFrame = glfwGetTime();
		deltaTime = static_cast<GLfloat>(currentFrame - lastFrame);
		lastFrame = currentFrame;

		glfwPollEvents();

		Update(deltaTime);
		Render();
	}

	Destroy();
	glfwTerminate();

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	static int spaceCount = 0;
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if(spaceCount == 0)
			glfwSetInputMode(g_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else if(spaceCount ==1)
			glfwSetInputMode(g_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		spaceCount = 1 - spaceCount;
	}

	g_pCamera->Camera_key_callback(window, key, scancode, action, mode);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	g_pCamera->Camera_mouse_callback(window, xpos, ypos);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	g_pCamera->Camera_scroll_callback(window, xoffset, yoffset);
	
}

glm::vec3 pointLightPositions[] = {
	glm::vec3(4.0f,  4.0f,  2.0f),
	glm::vec3(-4.0f, 4.0f, 2.0f),
	glm::vec3(4.0f,  -4.0f, -3.0f),
	glm::vec3(-4.0f,  -4.0f, -3.0f)
};

void InitResource()
{
	g_pShader = new Shader("..\\shaders\\BasicShader_vs.glsl", "..\\shaders\\BasicShader_ps.glsl");
	g_pLightObjectShader = new Shader("..\\shaders\\BasicShader_vs.glsl", "..\\shaders\\LightObjectShader_ps.glsl");
	g_pSingleColorShader = new Shader("..\\shaders\\BasicShader_vs.glsl", "..\\shaders\\SingleColor_ps.glsl");
	g_pGrassShader = new Shader("..\\shaders\\BasicShader_vs.glsl", "..\\shaders\\GrassShader_ps.glsl");
	g_pLineShader = new Shader("..\\shaders\\LineShader_vs.glsl", "..\\shaders\\SingleColor_ps.glsl");
	g_pScreensapceShader = new Shader("..\\shaders\\ScreenSpaceShader_vs.glsl", "..\\shaders\\BasicShader_ps.glsl");

	g_pQuadShape = new Shape(P3N3T2);
	g_pQuadShape->Create(&quadVertices[0], sizeof(quadVertices), numQuadVertex, &squadIndices[0], sizeof(squadIndices), numQuadIndex);

	g_pBoxShape = new BoxShape(P3N3T2);
	g_pBoxShape->Create(&BoxPTNVertices[0], sizeof(BoxPTNVertices), numBoxBTNVertex);

	float aspect = (float)g_nWidth / (float)g_nHeight;
	g_pCamera = new Camera(glm::vec3(0.0f, 5.0f, 15.0f), aspect);

	CreateTexture(g_RockTex, "..\\media\\racing-pack\\PNG\\Objects\\rock1.png");
	CreateTexture(g_ArrowTex, "..\\media\\racing-pack\\PNG\\Objects\\arrow_yellow.png");
	CreateTexture(g_WordTex, "..\\media\\letter_S.png");
	CreateTexture(g_ContainerD, "..\\media\\container2_diffuse.png");
	CreateTexture(g_ContainerS, "..\\media\\container2_specular.png");
	CreateTexture(g_ContainerE, "..\\media\\letter_S.png");//letter_S.png matrix.jpg
	CreateTexture(g_GrassTex, "..\\media\\grass.png");
	CreateTexture(g_GlassTex, "..\\media\\blending_transparent_window.png");

	g_pLight = new Light();
	g_pLight->BindShape(g_pBoxShape);
	g_pLight->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	g_pLight->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	g_pLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	g_pLight->lightPos = glm::vec3(-1.0f, 1.0f, 1.0f);

	g_pModel = new Model("../media/nanosuit/nanosuit.obj");

	// framebuffer
	g_pFrameBuffer = new FrameBuffer();
	g_pFrameBuffer->CreateFrameBuffer();

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

void Update(GLfloat dt)
{
	g_pCamera->Update(dt);
}

void Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, g_pFrameBuffer->fbo);

	glClearColor(0.0f, 0.25f, 0.47f, 1.0f); // state-setting /state-using
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	DrawLight();
	DrawGlass();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	PostProcess();

	glfwSwapBuffers(g_pWindow);
}

void Destroy()
{
	g_pQuadShape->ClearBuffer();
	delete g_pQuadShape; g_pQuadShape = nullptr;
	g_pBoxShape->ClearBuffer();
	delete g_pBoxShape; g_pBoxShape = nullptr;
	delete g_pShader;				g_pShader = nullptr;
	delete g_pLightObjectShader;	g_pLightObjectShader = nullptr;
	delete g_pCamera;				g_pCamera = nullptr;
	glDeleteTextures(1, &g_RockTex);	
	glDeleteTextures(1, &g_ArrowTex);
	glDeleteTextures(1, &g_WordTex);

}

void CreateTexture(GLuint& tex, const char* fileName)
{
	int width, height, chanels;
	unsigned char* image;
	image = SOIL_load_image(fileName, &width, &height, &chanels, SOIL_LOAD_RGBA);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(image);
}

void DrawGlass()
{
	float offset = static_cast<float>(sin(glfwGetTime()) * 10.0f);
	float angleR = static_cast<float>(glm::radians(glfwGetTime() * 100.0f));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	g_pShader->Use();

	glm::mat4 modelM;
	modelM = glm::translate(modelM, glm::vec3(offset, 5.0f, 0.0f));
	modelM = glm::rotate(modelM, angleR, glm::vec3(1.0f, 0.0f, 0.0f));
	modelM = glm::scale(modelM, glm::vec3(2.0f, 2.0f, 2.0f));
	
	g_pShader->vsParams.SetVSMatrix(modelM, g_pCamera->GetViewM(), g_pCamera->GetPerspProjM());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_GlassTex);
	glUniform1i(glGetUniformLocation(g_pShader->Program, "mainMap"), 0);

	g_pQuadShape->Draw();
	
	modelM = glm::mat4();
	modelM = glm::translate(modelM, glm::vec3(0.0f, 0.0f, 0.0f));
	modelM = glm::rotate(modelM, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelM = glm::scale(modelM, glm::vec3(20.0f));
	g_pShader->vsParams.SetModelMatrix(modelM);
	g_pQuadShape->Draw();

	modelM = glm::mat4();
	modelM = glm::translate(modelM, glm::vec3(-5.0f, 5.0f, 0.0f));
	modelM = glm::rotate(modelM, angleR, glm::vec3(1.0f, 0.0f, 0.0f));
	g_pShader->vsParams.SetModelMatrix(modelM);
	g_pBoxShape->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
}

void DrawLight()
{
	// draw light
	{
		g_pSingleColorShader->Use();
		glUniform4f(glGetUniformLocation(g_pSingleColorShader->Program, "drawColor"), 1.0f, 0.5f, 0.31f, 1.0f);
		
		glm::mat4 modelM;
		modelM = glm::translate(modelM, g_pLight->lightPos);
		modelM = glm::scale(modelM, glm::vec3(0.2f));
		
		g_pSingleColorShader->vsParams.SetVSMatrix(modelM, g_pCamera->GetViewM(), g_pCamera->GetPerspProjM());
		
		for (int i = 0; i < 4; ++i)
		{
			glm::mat4 modelM;
			modelM = glm::translate(modelM, pointLightPositions[i]);
			modelM = glm::scale(modelM, glm::vec3(0.2f));
			g_pSingleColorShader->vsParams.SetModelMatrix(modelM);

			g_pLight->pLightShape->Draw();
		}
	}
	
	// draw light scene
	{
		g_pLightObjectShader->Use();

		glm::mat4 modelM;
		modelM = glm::translate(modelM, glm::vec3(0.0f, 3.0f, 0.0f));
		modelM = glm::rotate(modelM, glm::radians((GLfloat)glfwGetTime() * 45.0f), glm::vec3(1.0f));
		modelM = glm::scale(modelM, glm::vec3(1.0f));
		
		g_pLightObjectShader->vsParams.SetVSMatrix(modelM, g_pCamera->GetViewM(), g_pCamera->GetPerspProjM());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_ContainerD);
		glUniform1i(glGetUniformLocation(g_pLightObjectShader->Program, "mat.diffuseMap0"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, g_ContainerS);
		glUniform1i(glGetUniformLocation(g_pLightObjectShader->Program, "mat.specularMap0"), 1);
		GLint matShininessLoc = glGetUniformLocation(g_pLightObjectShader->Program, "mat.shininess");
		glUniform1f(matShininessLoc, 32.0f);

		GLint viewPosLoc = glGetUniformLocation(g_pLightObjectShader->Program, "viewPos");
		glm::vec3 viewPos = g_pCamera->GetCameraPos();
		glUniform3f(viewPosLoc, viewPos.x, viewPos.y, viewPos.z);

		BindLightShaderParams(g_pLightObjectShader->Program);

		g_pBoxShape->Draw();

		modelM = glm::mat4();
		modelM = glm::translate(modelM, glm::vec3(5.0f, 0.0f, -2.0f));
		g_pLightObjectShader->vsParams.SetModelMatrix(modelM);
		g_pModel->Draw(*g_pLightObjectShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// draw grass
	{
		g_pGrassShader->Use();

		glm::vec3 grassPos[5] = 
		{
			glm::vec3(-1.5f,  0.5f, -0.48f),
			glm::vec3(1.5f,  0.5f,  0.51f),
			glm::vec3(0.0f,  0.5f,  0.7f),
			glm::vec3(-0.3f,  0.5f, -2.3f),
			glm::vec3(0.5f,  0.5f, -0.6f)
		};

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_GrassTex);
		glUniform1i(glGetUniformLocation(g_pGrassShader->Program, "grassMap"), 0);

		for (GLuint i = 0; i < 5; ++i)
		{
			glm::mat4 modelM;
			modelM = glm::translate(modelM, grassPos[i]);
			g_pGrassShader->vsParams.SetVSMatrix(modelM, g_pCamera->GetViewM(), g_pCamera->GetPerspProjM());
			g_pQuadShape->Draw();
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// draw line
	{
		glm::mat4 modelM;
		modelM = glm::translate(modelM, glm::vec3(0.0f, 0.0f, 0.0f));
		g_pLineShader->Use();
		g_pLineShader->vsParams.SetVSMatrix(modelM, g_pCamera->GetViewM(), g_pCamera->GetPerspProjM());
		glm::vec4 redColor(1.0f, 0.0f,0.0f,1.0f);
		glUniform4f(glGetUniformLocation(g_pLineShader->Program, "drawColor"), redColor.x, redColor.y, redColor.y, redColor.z);

		Line line;
		glm::vec3 linePoint[4] = 
		{
			glm::vec3(0.0f, 0.0f, 0.0f), 
			glm::vec3(0.0f,10.0f, 0.0f),
			glm::vec3(0.0f,10.0f, 0.0f),
			glm::vec3(10.0f,10.0f, 0.0f),
		};
		line.Create((&linePoint[0]), 2);
		line.Draw();
	}

}

void PostProcess()
{
	g_pScreensapceShader->Use();
	
	glBindVertexArray(quadVAO);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_pFrameBuffer->colorTexture);
	//glBindTexture(GL_TEXTURE_2D, g_GlassTex);
	glUniform1i(glGetUniformLocation(g_pScreensapceShader->Program, "mainMap"), 0);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);


}

void Ohter_Test()
{
	//glStencilMask(0x00);

	// draw other object

	//glStencilMask(0xFF);
	//glStencilFunc(GL_ALWAYS, 1, 0xFF);

	// draw Model

	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilMask(0x00);
	//glDisable(GL_DEPTH_TEST);
	// shaderProgram->Use()
	//{
		//draw Model Scale
	//}
	//glStencilMask(0xFF);
	//glEnable(GL_DEPTH_TEST);
}

void BindLightShaderParams(GLuint shaderProgram)
{
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.lightDir"), 1.0f, -1.0f, -1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
	// Point light 1
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].lightPos"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].linear"), 0.09);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].quadratic"), 0.032);
	// Point light 2
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].lightPos"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[1].linear"), 0.09);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[1].quadratic"), 0.032);
	// Point light 3
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].lightPos"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[2].linear"), 0.09);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[2].quadratic"), 0.032);
	// Point light 4
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].lightPos"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[3].linear"), 0.09);
	glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[3].quadratic"), 0.032);
	//// SpotLight
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.lightPos"), g_pCamera->cameraPos.x, g_pCamera->cameraPos.y, g_pCamera->cameraPos.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.lightDir"), g_pCamera->cameraFront.x, g_pCamera->cameraFront.y, g_pCamera->cameraFront.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.linear"), 0.09);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.quadratic"), 0.032);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.cutoff"), glm::cos(glm::radians(15.0f)));
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.outerCutoff"), glm::cos(glm::radians(30.0f)));
}
