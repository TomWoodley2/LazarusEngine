#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "Component.h"
#include "Rigidbody.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Component used to apply physics to collision objects

class RigidbodyComponent : public Component
{
private:
	float m_mass = 5.0f; // Will later be set in json file (in kg)
	glm::vec3 m_position;
	glm::quat m_orientation;
	glm::vec3 m_force = glm::vec3(0.0f, -9.8f, 0.0f);
	glm::vec3 m_velocity;
public:
	RigidbodyComponent(glm::vec3 posIn, const glm::quat orientationIn) : m_position(posIn), m_orientation(orientationIn) 
	{
		m_velocity = glm::vec3(1.0f,1.0f,0.0f);
	}
	void OnUpdate(float dt) override;
	void OnMessage(const std::string m) override;

	glm::vec3 getVelocity()
	{
		return m_velocity;
	}
};