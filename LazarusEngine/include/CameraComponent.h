#pragma once
#include "Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

class CameraComponent : public Component
{
public:
	glm::vec3 m_position;
	glm::quat m_orientation;
	float m_fov;
	
	CameraComponent() : m_position(0), m_orientation(1,0,0,0), m_fov(50) 
	{
	}
	
	CameraComponent(const glm::vec3& pos) : m_position(pos), m_orientation(1, 0, 0, 0),m_fov(50) {}
	CameraComponent(const glm::vec3& pos, const glm::quat& orient) : m_position(pos), m_orientation(orient), m_fov(50) {}

	void lookAt(const glm::vec3& target) { m_orientation = (glm::toQuat(glm::lookAt(m_position, target, glm::vec3(0, 1, 0)))); }
	
	const glm::vec3& position() const { return m_position; }
	const glm::quat& orientation() const { return m_orientation; }

	// added setters
	void setPosition(const glm::vec3 newPosition)
	{
		m_position = newPosition;
	}
	void setOrientation(const glm::quat newOrientation)
	{
		m_orientation = newOrientation;

	}

	glm::mat4 getViewMatrix() const 
	{ 
		// try this....
		return glm::translate(glm::mat4_cast(inverse(m_orientation)), -m_position); 
	}
	
#if 0

	// if camera pos and orientation are copied from transform then these are not used

	void translate(const glm::vec3 &v) { m_position += v * m_orientation; }
	void translate(float x, float y, float z) { m_position += glm::vec3(x, y, z) * m_orientation; }

	void rotate(float angle, const glm::vec3 &axis) { m_orientation *= glm::angleAxis(angle, axis * m_orientation); }
	void rotate(float angle, float x, float y, float z) { m_orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orientation); }

	void yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f); }
	void pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f); }
	void roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f); }
#endif
	void setFOV(float fov) { m_fov = fov; }


	// component interface - not used currently

	void OnUpdate(float dt) override
	{
	}
	void OnMessage(const std::string m) override
	{

	}

};
