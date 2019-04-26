#pragma once

#include <glm/vec3.hpp>
//#include <glm/mat4x4.hpp> // Might need 3x3?

struct Rigidbody
{
	float mass = 1.0f; // Mass of the object
	bool gravityEnabled = true; // does the object use gravity
	bool positionLocked = false; // Lock the positon of the object (might be replaced by using a high mass in calculations later on
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f); // Initial velocity of the object
	float bounceCoefficient = 1.0f; // How bouncy the object is - defaults to 1.0

};
