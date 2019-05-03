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

	// Getters
	glm::vec3 getNegativeCorner();
	glm::vec3 getPositiveCorner();

private:

	std::vector<Mesh> v_meshes;

	// Having these at 0,0,0 does mean some part of the object collision box has to be in the centre of mass
	glm::vec3 negativeCorner = glm::vec3(0.0f, 0.0f, 0.0f); // -x, -y, -z corner of mesh
	glm::vec3 positiveCorner = glm::vec3(0.0f, 0.0f, 0.0f); // +x, +y, +z corner of mesh

	string directory;
	vector<Texture> v_textures;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
	                                     std::string typeName);
};
