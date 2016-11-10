
#ifndef _MODEL_H
#define _MODEL_H

#include<GL\glew.h>
#include <GLFW\glfw3.h>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"

class Model
{
public:
	Model(GLchar* path);

	void Draw(const Shader& shader);

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> texture_loaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};

GLint CreateTextureFromFile(const char* path, std::string directory);

#endif // !_MODEL_H
