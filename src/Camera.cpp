
#include "Camera.h"

Camera::Camera(glm::vec3 cameraPos, GLfloat aspect)
{
	this->cameraPos = cameraPos;
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	pitch = 0.0f;
	yaw = -90.0f;
	lastX = 400;
	lastY = 300;
	fov = 45.0f;
	this->aspect = aspect;
	bFirstMouse = GL_TRUE;

	for (GLint i = 0; i < 4; ++i)
		keys[i] = GL_FALSE;

	orthoProjM = glm::ortho(-this->aspect, this->aspect, -1.0f, 1.0f, 0.1f, 100.0f);
	perspProjM = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
}

void Camera::Update(GLfloat dt)
{
	GLfloat cameraSpeed = dt * 5.0f;
	if (keys[UP])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[DOWN])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[LEFT])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[RIGHT])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

glm::mat4 Camera::GetViewM()
{
	viewM = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	return viewM;
}
glm::mat4 Camera::GetOrthoProjM()
{
	return orthoProjM;
}
glm::mat4 Camera::GetPerspProjM()
{
	return perspProjM;
}
glm::vec3 Camera::GetCameraPos()
{
	return cameraPos;
}

void Camera::Camera_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_W)
	{
		if (action == GLFW_PRESS)
			keys[UP] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			keys[UP] = GL_FALSE;
	}
	else if (key == GLFW_KEY_S)
	{
		if (action == GLFW_PRESS)
			keys[DOWN] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			keys[DOWN] = GL_FALSE;
	}
	else if (key == GLFW_KEY_A)
	{
		if (action == GLFW_PRESS)
			keys[LEFT] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			keys[LEFT] = GL_FALSE;
	}
	else if (key == GLFW_KEY_D)
	{
		if (action == GLFW_PRESS)
			keys[RIGHT] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			keys[RIGHT] = GL_FALSE;
	}
}
void Camera::Camera_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (bFirstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		bFirstMouse = GL_FALSE;
	}

	GLfloat xoffset = static_cast<GLfloat>(xpos - lastX);
	GLfloat yoffset = static_cast<GLfloat>(ypos - lastY);
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	glm::clamp(yaw, -89.0f, 89.0f);
	glm::clamp(pitch, -89.0f, 89.0f);

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}
void Camera::Camera_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= static_cast<GLfloat>(yoffset);
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}