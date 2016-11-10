
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <string>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

struct Material_Color
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
};

struct Material_Map
{
	GLuint diffuseMap;
	GLuint specularMap;
	GLuint emissionMap;

	GLfloat shininess;
};



#endif // !_MATERIAL_H_
