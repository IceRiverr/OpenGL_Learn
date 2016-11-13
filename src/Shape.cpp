
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
	/*glGenBuffers(1, &meshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
	if (bHasNormal)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (positions.size() * 3 + normals.size() * 3 + texcoords.size() * 2), nullptr, GL_STATIC_DRAW);
		
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * (positions.size() * 3), &positions[0]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (positions.size() * 3), sizeof(GLfloat) * (normals.size() * 3), &normals[0]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (positions.size() * 3 + normals.size() * 3), sizeof(GLfloat) * (texcoords.size() * 2), &texcoords[0]);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (positions.size() * 3 + texcoords.size() * 2), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * (positions.size() * 3), &positions[0]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (positions.size() * 3), sizeof(GLfloat) * (texcoords.size() * 2), &texcoords[0]);
	}

	glGenBuffers(1, &meshEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &meshVAO);
	glBindVertexArray(meshVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * (3 *positions.size())));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * (3 * positions.size() + 3 * sizeof(normals.size()))));

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	GLfloat data[24] = 
	{
		0.0f, 0.0f, 0.0f,	 0.0f, 0.0f, 1.0f,	 0.0f, 0.0f,
		4.0f, 0.0f, 0.0f,	 0.0f, 0.0f, 1.0f,	 2.0f, 0.0f,
		2.0f, 2.0f, 0.0f,	 0.0f, 0.0f, 1.0f,	 1.0f, 1.0f,
	};

	GLfloat positions[9] = 
	{
		0.0f, 0.0f, 0.0f,
		4.0f, 0.0f, 0.0f,
		2.0f, 2.0f, 0.0f
	};

	GLfloat normals[9] =
	{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	GLfloat texCoords[6] =
	{
		0.0f, 0.0f,
		2.0f, 0.0f,
		1.0f, 1.0f,
	};

	GLuint vertexCount = 3;

	std::vector<glm::vec3> positionsV(vertexCount);
	std::vector<glm::vec3> normalsV(vertexCount);
	std::vector<glm::vec2> texCoordsV(vertexCount);
	memcpy(&positionsV[0], &positions[0], sizeof(positions));
	memcpy(&normalsV[0], &normals[0], sizeof(normals));
	memcpy(&texCoordsV[0], &texCoords[0], sizeof(texCoords));

	GLuint ind[3] = {0,1,2};

	glGenBuffers(1, &meshVBO);
	glGenBuffers(1, &meshEBO);
	glGenVertexArrays(1, &meshVAO);

	glBindVertexArray(meshVAO);
	glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
	
	//glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);
	
	//glBufferData(GL_ARRAY_BUFFER, sizeof(data), nullptr, GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(normals), normals);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(normals), sizeof(texCoords), texCoords);

	glBufferData(GL_ARRAY_BUFFER, sizeof(data), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0,																				positionsV.size() * sizeof(glm::vec3),	&positionsV[0]);
	glBufferSubData(GL_ARRAY_BUFFER, positionsV.size() * sizeof(glm::vec3),											normalsV.size() * sizeof(glm::vec3),	&normalsV[0]);
	glBufferSubData(GL_ARRAY_BUFFER, positionsV.size() * sizeof(glm::vec3) + normalsV.size() * sizeof(glm::vec3),	texCoordsV.size() * sizeof(glm::vec2),	&texCoordsV[0]);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), &ind[0],GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(positions)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(positionsV.size() * sizeof(glm::vec3)));

	glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(sizeof(positions) + sizeof(normals)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(positionsV.size() * sizeof(glm::vec3) + normalsV.size() * sizeof(glm::vec3)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TestMesh::Draw()
{
	glBindVertexArray(meshVAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
