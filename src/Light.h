
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Shape.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Light
{
public:
	BoxShape* pLightShape;

	void BindShape(BoxShape* light);

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 lightPos;
};

static const int LIGNT_COUNT = 4;

glm::vec3 GetLightPosition(GLuint index);

class Camera;
void BindLightShaderParams(GLuint shaderProgram, Camera* camera);

#endif // !_LIGHT_H_
