#pragma once

#include "glm/vec3.hpp"

class Collision
{
private:
	char closestPlane = 'N'; // Closest plane
	int planeValue = 0; // Value of the plane

public:
	Collision(); // Constructor
	bool checkAABBCollision(glm::vec3 mesh1NegIn, glm::vec3 mesh1PosIn, glm::vec3 mesh2NegIn, glm::vec3 mesh2PosIn); // Checking for axis alligned bounding collisions
	
	

	// Getting values - always used just after sucessful collision to get values
	char getClosestPlane();
	int getPlaneValue();
};