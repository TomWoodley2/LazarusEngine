#pragma once
#include <string>
#include <vector>
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model
{
public:

	Model(string filepath);

	void render(const unsigned int shaderProgram);

	static unsigned int TextureFromFile(const char* filepath, const string& directory, bool gamma = false);

private:

	std::vector<Mesh> v_meshes;
	string directory;
	vector<Texture> v_textures;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
	                                     std::string typeName);
};
