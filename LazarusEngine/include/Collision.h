#pragma once

#include "glm/vec3.hpp"

class Collision
{
private:

public:
	Collision(); // Constructor
	bool checkAABBCollision(glm::vec3 mesh1NegIn, glm::vec3 mesh1PosIn, glm::vec3 mesh2NegIn, glm::vec3 mesh2PosIn); // Checking for axis alligned bounding collisions
};