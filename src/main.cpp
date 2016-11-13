
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
#include "Skybox.h"
#include "Tookit.h"
#include "Postprocess.h"

Shader* g_pShader = nullptr;
Shader* g_pLightObjectShader = nullptr;
Shader* g_pSingleColorShader = nullptr;
Shader* g_pGrassShader = nullptr;
Shader* g_pLineShader = nullptr;
Shader* g_pEnvMappingShader = nullptr;

Shape* g_pQuadShape = nullptr;
BoxShape* g_pBoxShape = nullptr;
Model* g_pModel = nullptr;
Model* g_pBunnyModel = nullptr;

Camera* g_pCamera = nullptr;
GLFWwindow* g_pWindow;
Light* g_pLight;
FrameBuffer* g_pFrameBuffer;
Skybox* g_pSkybox;
Postprocess* g_pPostprocess;

GLint g_nWidth, g_nHeight;
GLuint g_RockTex;
GLuint g_ArrowTex;
GLuint g_WordTex;
GLuint g_GrassTex;
GLuint g_GlassTex;

GLuint g_ContainerD;
GLuint g_ContainerS;
GLuint g_ContainerE;

TestMesh* g_pTestMesh;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode); 
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void InitResource();
void Update(GLfloat dt);
void Render();
void Destroy();

void DrawGlass(Camera* camera);
void DrawLight(Camera* camera);

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

	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);*/
	
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

void InitResource()
{
	g_pShader = new Shader("..\\shaders\\BasicShader_vs.glsl", "..\\shaders\\BasicShader_ps.glsl");
	g_pLightObjectShader = new Shader("..\\shaders\\BasicShader_vs.glsl", "..\\shaders\\LightObjectShader_ps.glsl");
	g_pSingleColorShader = new Shader("..\\shaders\\BasicShader_vs.glsl", "..\\shaders\\SingleColor_ps.glsl");
	g_pGrassShader = new Shader("..\\shaders\\BasicShader_vs.glsl", "..\\shaders\\GrassShader_ps.glsl");
	g_pLineShader = new Shader("..\\shaders\\LineShader_vs.glsl", "..\\shaders\\SingleColor_ps.glsl");
	g_pEnvMappingShader = new Shader("..\\shaders\\BasicShader_vs.glsl", "..\\shaders\\EnvironmentMapping_ps.glsl");
	
	g_pQuadShape = new Shape(P3N3T2);
	g_pQuadShape->Create(&quadVertices[0], sizeof(quadVertices), numQuadVertex, &squadIndices[0], sizeof(squadIndices), numQuadIndex);

	g_pBoxShape = new BoxShape(P3N3T2);
	g_pBoxShape->Create(&BoxPTNVertices[0], sizeof(BoxPTNVertices), numBoxBTNVertex);

	float aspect = (float)g_nWidth / (float)g_nHeight;
	g_pCamera = new Camera(glm::vec3(0.0f, 5.0f, 15.0f), aspect);

	g_RockTex = LoadTextureFromFile("..\\media\\racing-pack\\PNG\\Objects\\rock1.png");
	g_ArrowTex = LoadTextureFromFile("..\\media\\racing-pack\\PNG\\Objects\\arrow_yellow.png");
	g_WordTex = LoadTextureFromFile("..\\media\\letter_S.png");
	g_ContainerD = LoadTextureFromFile("..\\media\\container2_diffuse.png");
	g_ContainerS = LoadTextureFromFile("..\\media\\container2_specular.png");
	g_ContainerE = LoadTextureFromFile("..\\media\\letter_S.png");
	g_GrassTex = LoadTextureFromFile("..\\media\\grass.png");
	g_GlassTex = LoadTextureFromFile("..\\media\\blending_transparent_window.png");
	
	g_pLight = new Light();
	g_pLight->BindShape(g_pBoxShape);
	g_pLight->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	g_pLight->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	g_pLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	g_pLight->lightPos = glm::vec3(-1.0f, 1.0f, 1.0f);

	g_pModel = new Model("../media/nanosuit/nanosuit.obj");
	//g_pBunnyModel = new Model("../media/StanfordModel/bunny.ply");

	g_pFrameBuffer = new FrameBuffer();
	g_pFrameBuffer->CreateFrameBuffer();

	g_pSkybox = new Skybox();
	g_pSkybox->Init();
	std::string skyboxFiles[6]
	{
		"..\\media\\skybox\\right.jpg",
		"..\\media\\skybox\\left.jpg",
		"..\\media\\skybox\\top.jpg",
		"..\\media\\skybox\\bottom.jpg",
		"..\\media\\skybox\\back.jpg",
		"..\\media\\skybox\\front.jpg",
	};
	std::vector<std::string> skyboxFaces(6);
	for (int i = 0; i < 6; ++i)
		skyboxFaces[i] = skyboxFiles[i];
	g_pSkybox->SetCubeMapFiles(skyboxFaces);

	g_pPostprocess = new Postprocess();
	g_pPostprocess->Init();

	g_pTestMesh = new TestMesh();
	g_pTestMesh->Create();
}

void Update(GLfloat dt)
{
	g_pCamera->Update(dt);
}

void Render()
{
	// fornt view
	glBindFramebuffer(GL_FRAMEBUFFER, g_pFrameBuffer->fbo);
	glClearColor(0.0f, 0.25f, 0.47f, 1.0f); // state-setting /state-using
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	DrawLight(g_pCamera);
	g_pSkybox->Draw(g_pCamera);
	DrawGlass(g_pCamera);

	// postProcess
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	g_pPostprocess->Draw(g_pFrameBuffer->colorTexture);

	glfwSwapBuffers(g_pWindow);
}

void Destroy()
{
	g_pQuadShape->ClearBuffer();
	g_pBoxShape->ClearBuffer();
	delete g_pQuadShape;			g_pQuadShape = nullptr;
	delete g_pBoxShape;				g_pBoxShape = nullptr;
	delete g_pShader;				g_pShader = nullptr;
	delete g_pLightObjectShader;	g_pLightObjectShader = nullptr;
	delete g_pCamera;				g_pCamera = nullptr;
	delete g_pSkybox;				g_pSkybox = nullptr;
	delete g_pPostprocess;			g_pPostprocess = nullptr;
	glDeleteTextures(1, &g_RockTex);
	glDeleteTextures(1, &g_ArrowTex);
	glDeleteTextures(1, &g_WordTex);
}

void DrawGlass(Camera* camera)
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
	
	g_pShader->vsParams.SetVSMatrix(modelM, camera->GetViewM(), camera->GetPerspProjM());

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

void DrawLight(Camera* camera)
{
	// draw light
	{
		g_pSingleColorShader->Use();
		glUniform4f(glGetUniformLocation(g_pSingleColorShader->Program, "drawColor"), 1.0f, 0.5f, 0.31f, 1.0f);
		
		glm::mat4 modelM = glm::mat4();
		
		g_pSingleColorShader->vsParams.SetVSMatrix(modelM, camera->GetViewM(), camera->GetPerspProjM());
		
		for (int i = 0; i < LIGNT_COUNT; ++i)
		{
			modelM = glm::mat4();
			modelM = glm::translate(modelM, GetLightPosition(i));
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
		
		g_pLightObjectShader->vsParams.SetVSMatrix(modelM, camera->GetViewM(), camera->GetPerspProjM());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_ContainerD);
		glUniform1i(glGetUniformLocation(g_pLightObjectShader->Program, "mat.diffuseMap0"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, g_ContainerS);
		glUniform1i(glGetUniformLocation(g_pLightObjectShader->Program, "mat.specularMap0"), 1);
		GLint matShininessLoc = glGetUniformLocation(g_pLightObjectShader->Program, "mat.shininess");
		glUniform1f(matShininessLoc, 32.0f);

		GLint viewPosLoc = glGetUniformLocation(g_pLightObjectShader->Program, "viewPos");
		glm::vec3 viewPos = camera->GetCameraPos();
		glUniform3f(viewPosLoc, viewPos.x, viewPos.y, viewPos.z);

		BindLightShaderParams(g_pLightObjectShader->Program, camera);

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
			g_pGrassShader->vsParams.SetVSMatrix(modelM, camera->GetViewM(), camera->GetPerspProjM());
			g_pQuadShape->Draw();
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// draw line
	{
		g_pLineShader->Use();

		glm::mat4 modelM;
		modelM = glm::translate(modelM, glm::vec3(0.0f, 0.0f, 0.0f));
		g_pLineShader->vsParams.SetVSMatrix(modelM, camera->GetViewM(), camera->GetPerspProjM());
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

	{
		g_pEnvMappingShader->Use();
		
		glm::mat4 modelM;
		modelM = glm::translate(modelM, glm::vec3(-5.0f, 1.0f, 0.0f));
		modelM = glm::scale(modelM, glm::vec3(1.0f, 1.0f, 1.0f));
		g_pEnvMappingShader->vsParams.SetVSMatrix(modelM, camera->GetViewM(), camera->GetPerspProjM());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, g_pSkybox->GetSkyBoxMap());
		glUniform1i(glGetUniformLocation(g_pEnvMappingShader->Program, "envMap"), 0);

		glm::vec3 cameraPos = camera->GetCameraPos();
		glUniform3f(glGetUniformLocation(g_pEnvMappingShader->Program, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);

		g_pBoxShape->Draw();
		//g_pBunnyModel->Draw(*g_pEnvMappingShader);
	}

	{
		g_pSingleColorShader->Use();
		glm::mat4 modelM;
		modelM = glm::translate(modelM, glm::vec3(10.0f, 1.0f, 0.0f));
		modelM = glm::scale(modelM, glm::vec3(1.0f, 1.0f, 1.0f));
		g_pSingleColorShader->vsParams.SetVSMatrix(modelM, camera->GetViewM(), camera->GetPerspProjM());

		g_pTestMesh->Draw();
	}
}
