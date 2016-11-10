
#include "Model.h"
#include <SOIL.h>

Model::Model(GLchar* path)
{
	this->loadModel(path);
}

void Model::Draw(const Shader& shader)
{
	for (GLuint i = 0; i < meshes.size(); ++i)
		this->meshes[i].Draw(shader);
}

void Model::loadModel(std::string path)
{
	Assimp::Importer import;

	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
	}
	this->directory = path.substr(0, path.find_last_of('/'));

	this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh,scene));
	}

	for (GLuint i = 0; i < node->mNumChildren; ++i)
	{
		this->processNode(node->mChildren[i],scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;

		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoord = vec;
		}
		else
			vertex.TexCoord = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMap = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuseMap");
		textures.insert(textures.end(), diffuseMap.begin(), diffuseMap.end());

		std::vector<Texture> specularMap = this->loadMaterialTextures(material,aiTextureType_SPECULAR, "specularMap");
		textures.insert(textures.end(), specularMap.begin(), specularMap.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string texType)
{
	std::vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		GLboolean bSkip = false;
		for (GLuint j = 0; j < texture_loaded.size(); ++j)
		{
			if (texture_loaded[j].path == str)
			{
				textures.push_back(texture_loaded[j]);
				bSkip = true;
				break;
			}
		}

		if (!bSkip)
		{
			Texture texture;
			texture.id = CreateTextureFromFile(str.C_Str(), this->directory);
			texture.type = texType;
			texture.path = str;
			textures.push_back(texture);
			this->texture_loaded.push_back(texture);
		}
	}
	return textures;
}

GLint CreateTextureFromFile(const char* path, std::string directory)
{
	string filename = string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	return textureID;
}
