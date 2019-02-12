#pragma once

//#include <glad/glad.h> 
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
using namespace std;

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

struct Texture {
	unsigned int id;
	string type;
	aiString filepath;
};

class Mesh 
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	// render the mesh with a given shader program
	void render(const unsigned int shaderProgram);

private:
	// buffer objects
	unsigned int VBO, EBO;

	// initialises all the buffer arrays
	void setupMesh();
	
};