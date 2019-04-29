#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "Component.h"
#include "Rigidbody.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Rigidbody.h"

// Component used to apply physics to collision objects

class RigidbodyComponent : public Component
{
private:
	float m_mass = 0.5f; // Will later be set in json file (in kg)
	float m_bounce_coefficient = 0.8f; // How bouncy the object is (1 = full bounce, 0 = no bounce)
	glm::vec3 m_position;
	glm::quat m_orientation;
	glm::vec3 m_acceleration = glm::vec3(0.0f, -9.8f, 0.0f);
	glm::vec3 m_velocity;
	glm::vec3 m_force = glm::vec3(0.0f, 0.0f, 0.0f);

	bool isUsingGravity = true; // is the object affected by gravity
	//glm::vec3 m_acceleration = glm::vec3(0.0f, -9.8f, 0.0f);
public:
	RigidbodyComponent(glm::vec3 posIn, const glm::quat orientationIn, Rigidbody rigidbodyIn) : m_position(posIn), m_orientation(orientationIn)
	{
		m_velocity = rigidbodyIn.velocity;
		m_mass = rigidbodyIn.mass;
		m_bounce_coefficient = rigidbodyIn.bounceCoefficient;
		isUsingGravity = rigidbodyIn.gravityEnabled;

		if (isUsingGravity)
		{
			m_force = glm::vec3(0.0f, -9.8f, 0.0f); // Use gravity
		}
		else
		{
			m_force = glm::vec3(0.0f, 0.0f, 0.0f); // Don't use gravity
		}

		//m_force = m_mass * m_acceleration;
	}

	void OnUpdate(float dt) override;
	void OnMessage(const std::string m) override;

	float getMass()
	{
		return m_mass;
	}

	glm::vec3 getVelocity()
	{
		return m_velocity;
	}
	glm::vec3 getAcceleration()
	{
		return m_acceleration;
	}
	glm::vec3 getForce()
	{
		return m_force;
	}

	float getBounceCoefficient()
	{
		return m_bounce_coefficient;
	}

	void setVelocity(glm::vec3 velocityIn)
	{
		m_velocity = velocityIn;
	}

	void setAcceleration(glm::vec3 accelerationIn)
	{
		m_acceleration = accelerationIn;
	}

	void setForce(glm::vec3 forceIn)
	{
		m_force = forceIn;
	}

	void setBounceCoefficient(float bounceCoefficientIn)
	{
		m_bounce_coefficient = bounceCoefficientIn;
	}

	void setRigidbody(Rigidbody rigidbodyIn)
	{
		m_velocity = rigidbodyIn.velocity;
		m_mass = rigidbodyIn.mass;
		m_bounce_coefficient = rigidbodyIn.bounceCoefficient;
		isUsingGravity = rigidbodyIn.gravityEnabled;

		if (isUsingGravity)
		{
			m_force = glm::vec3(0.0f, -9.8f, 0.0f); // Use gravity
		}
		else
		{
			m_force = glm::vec3(0.0f, 0.0f, 0.0f); // Don't use gravity
		}

		//m_force = m_mass * m_acceleration;
	}
};