
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include<GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Camera
{
public:
	enum MoveDir
	{
		UP = 0,
		DOWN,
		LEFT,
		RIGHT,
	};

	Camera(glm::vec3 cameraPos, GLfloat aspect);
	void Update(GLfloat dt);
	glm::mat4 GetViewM();
	glm::mat4 GetOrthoProjM();
	glm::mat4 GetPerspProjM();
	glm::vec3 GetCameraPos();

	void Camera_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void Camera_mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void Camera_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	GLfloat pitch;
	GLfloat yaw;
private:
	glm::vec3 cameraUp;
	
	GLdouble lastX, lastY;
	GLfloat fov;
	GLfloat aspect;
	GLboolean keys[4];
	GLboolean bFirstMouse;

	glm::mat4 viewM;
	glm::mat4 orthoProjM;
	glm::mat4 perspProjM;
};

#endif // !_CAMERA_H_
