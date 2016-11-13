
#ifndef _SHAPE_H_
#define _SHAPE_H_

#include<GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <vector>

enum VertexLayout
{
	P3 = 0,	// pos for point/line	
	P3T2,	// pos texcoord
	P3N3T2	// pos texcoord normalize
};

void CreateVertexLayout(const VertexLayout& layout,GLuint& shapeVAO, const GLuint& shapeVBO, const GLuint& shapeEBO = -1);

class Shape
{
public:
	Shape(VertexLayout layout = P3T2);
	~Shape() {}

	void Create(GLfloat* vertexData, GLuint numVertexByte,GLuint numVertex, GLuint* indexData, GLuint numIndexByte, GLuint numIndex);
	void ClearBuffer();
	void Draw();

	GLuint shapeVBO;
	GLuint shapeEBO;
	GLuint shapeVAO;
	VertexLayout layout;

	GLuint NumVertex;
	GLuint NumIndex;
};

class BoxShape
{
public:
	BoxShape(VertexLayout layout = P3T2);
	~BoxShape() {}
	void Create(GLfloat* vertexData, GLuint numVertexByte, GLuint numVertex);
	void ClearBuffer();
	void Draw();

	GLuint shapeVBO;
	GLuint shapeVAO;
	VertexLayout layout;
	
	GLuint NumVertex;
};

class Line
{
public:
	Line() {}
	~Line();
	void Create(GLfloat* lineList, GLuint numLine);
	void Create(glm::vec3* lineList, GLuint numLine);
	void Draw();

	GLuint lineVBO;
	GLuint lineVAO;

	GLuint numLine;
};

class LineList
{
public:
	LineList() {}
	~LineList();
	void Create(GLfloat* pointList, GLuint numPoint);
	void Draw();

	GLuint lineVBO;
	GLuint lineVAO;

	GLuint numPoint;
};

class TestMesh
{
public:
	void Create();

	void Draw();
	
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<GLuint> indices;

private:
	GLuint meshVAO;
	GLuint meshVBO;
	GLuint meshEBO;
};

#endif // !_MESH_H_
