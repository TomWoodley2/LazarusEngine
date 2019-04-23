#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp> // Might need 3x3?

struct Rigidbody
{
	// Constants
	double mass;

	// State variables
	glm::vec3 position;
	glm::mat4 rotation;

};
