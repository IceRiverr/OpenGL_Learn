
#include "Shape.h"

Shape::Shape(VertexLayout layout)
{
	this->layout = layout;
}

void Shape::Create(GLfloat* vertexData, GLuint numVertexByte, GLuint numVertex, GLuint* indexData, GLuint numIndexByte, GLuint numIndex)
{
	this->NumVertex = numVertex;
	this->NumIndex = numIndex;

	glGenBuffers(1, &shapeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
	glBufferData(GL_ARRAY_BUFFER, numVertexByte, vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &shapeEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shapeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndexByte, indexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &shapeVAO);
	CreateVertexLayout(layout, shapeVAO, shapeVBO, shapeEBO);
	
}

void Shape::ClearBuffer()
{
	glDeleteBuffers(1, &shapeVBO);
	glDeleteBuffers(1, &shapeEBO);
	glDeleteVertexArrays(1, &shapeVAO);
}

void Shape::Draw()
{
	glBindVertexArray(shapeVAO);
	glDrawElements(GL_TRIANGLES, NumIndex, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

BoxShape::BoxShape(VertexLayout layout)
{
	this->layout = layout;
}

void BoxShape::Create(GLfloat* vertexData, GLuint numVertexByte, GLuint numVertex)
{
	this->NumVertex = numVertex;

	glGenBuffers(1,&shapeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
	glBufferData(GL_ARRAY_BUFFER, numVertexByte, vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &shapeVAO);
	CreateVertexLayout(layout, shapeVAO, shapeVBO, -1);

}
void BoxShape::ClearBuffer()
{
	glDeleteBuffers(1, &shapeVBO);
	glDeleteVertexArrays(1, &shapeVAO);
}
void BoxShape::Draw()
{
	glBindVertexArray(shapeVAO);

	glDrawArrays(GL_TRIANGLES, 0, NumVertex);
	glBindVertexArray(0);
}

void CreateVertexLayout(const VertexLayout& layout, GLuint& shapeVAO, const GLuint& shapeVBO, const GLuint& shapeEBO)
{
	switch (layout)
	{
	case P3:
		glBindVertexArray(shapeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		break;

	case P3T2:
		glBindVertexArray(shapeVAO);
		{
			glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
			if(shapeEBO != -1)
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shapeEBO);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (shapeEBO != -1)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		break;
	case P3N3T2:
		glBindVertexArray(shapeVAO);
		{
			glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
			if (shapeEBO != -1)
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shapeEBO);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (shapeEBO != -1)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		break;
	default:
		break;
	}
}

Line::~Line()
{
	glDeleteBuffers(1, &lineVBO);
	glDeleteVertexArrays(1, &lineVAO);
}

void Line::Create(GLfloat* lineList, GLuint numLine)
{
	this->numLine = numLine;
	
	glGenBuffers(1, &lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat) * this->numLine * 6, lineList, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &lineVAO);
	CreateVertexLayout(VertexLayout::P3, lineVAO, lineVBO);
}

void Line::Create(glm::vec3* pointList, GLuint numPoint)
{
	Create((GLfloat*)pointList, numPoint);
}

void Line::Draw()
{
	glBindVertexArray(lineVAO);
	glDrawArrays(GL_LINES, 0, numLine * 2);
	glBindVertexArray(0);
}

LineList::~LineList()
{
	glDeleteBuffers(1, &lineVBO);
	glDeleteVertexArrays(1, &lineVAO);
}

void LineList::Create(GLfloat* pointList, GLuint numPoint)
{
	this->numPoint = numPoint;

	glGenBuffers(1, &lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numPoint * 3, pointList, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &lineVAO);
	CreateVertexLayout(VertexLayout::P3, lineVAO, lineVBO);
}
void LineList::Draw()
{
	glBindVertexArray(lineVAO);
	glDrawArrays(GL_LINE_STRIP, 0, numPoint);
	glBindVertexArray(0);
}

void TestMesh::Create()
{
	GLfloat _positions[] = 
	{
		0.0f, 0.0f, 0.0f,
		4.0f, 0.0f, 0.0f,
		2.0f, 2.0f, 0.0f,
		0.0f, 2.0f, 0.0f,
	};

	GLfloat _normals[] =
	{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	GLfloat _texCoords[] =
	{
		0.0f, 0.0f,
		2.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

	GLuint _indices[] = 
	{
		0,1,2 ,
		0,2,3,
	};

	GLuint vertexCount = 4;
	positions.resize(vertexCount);
	normals.resize(vertexCount);
	texcoords.resize(vertexCount);
	indices.resize(sizeof(_indices) / sizeof(GLuint));

	memcpy(&positions[0], &_positions[0], sizeof(_positions));
	memcpy(&normals[0], &_normals[0], sizeof(_normals));
	memcpy(&texcoords[0], &_texCoords[0], sizeof(_texCoords));
	memcpy(&indices[0], &_indices[0], sizeof(_indices));

	glGenBuffers(1, &meshVBO);
	glGenBuffers(1, &meshEBO);
	glGenVertexArrays(1, &meshVAO);

	glBindVertexArray(meshVAO);
	glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);

	GLuint numBufferByte1 = positions.size() * sizeof(glm::vec3) + normals.size() * sizeof(glm::vec3) + texcoords.size() * sizeof(glm::vec2);

	glBufferData(GL_ARRAY_BUFFER, numBufferByte1, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0,																			positions.size() * sizeof(glm::vec3),	&positions[0]);
	glBufferSubData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3),										normals.size() * sizeof(glm::vec3),		&normals[0]);
	glBufferSubData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3) + normals.size() * sizeof(glm::vec3), texcoords.size() * sizeof(glm::vec2),	&texcoords[0]);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0],GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(positions.size() * sizeof(glm::vec3)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(positions.size() * sizeof(glm::vec3) + normals.size() * sizeof(glm::vec3)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TestMesh::Draw()
{
	glBindVertexArray(meshVAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
