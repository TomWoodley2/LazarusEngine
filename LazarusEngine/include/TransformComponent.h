#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

// possible answer to the roll problem
// https://stackoverflow.com/questions/20855797/quaternion-based-camera-unwanted-roll

// use this define to switch between old implementation and new one keeping yaw and pitch seperate 
#define NO_ROLL (1)

//quat from euler angles - work with that later?
//q = qyaw qpitch qroll where:  (NOTE ORDER MATTERS)
//qroll = [cos(y / 2), (sin(y / 2), 0, 0)]
//	qpitch = [cos(q / 2), (0, sin(q / 2), 0)]
//	qyaw = [cos(f / 2), (0, 0, sin(f / 2)]

class TransformComponent : public Component
{

	// values for HP home desktop
	//const float moveStepsize = 1.0f / 256.0f; // 0.004f;  accurate binary value

	//const float rotateStepsize = 1.0f / 512.0f;	// 0.001f;

	const float moveStepsize = 1.0f / 32.0f; // 0.004f;  accurate binary value
	const float rotateStepsize = 1.0f / 98.0f;	// 0.001f;

public:
	glm::vec3 m_position;
	glm::vec3 m_scale;

private:
#if NO_ROLL

	glm::quat m_orientationYaw;
	glm::quat m_orientationPitch;

#else

	glm::quat m_orientation;

#endif

public:


	glm::quat getOrientation()
	{
#if NO_ROLL
		return m_orientationYaw * m_orientationPitch;
#else
		return m_orientation;
#endif
	}


	void OnUpdate(float dt) override{}
	void OnMessage(const std::string m) override
	{
		// e.g.	if (m == "rotateLeft")
		if (m == "rotateLeft")
		{
			yaw(rotateStepsize);		// anti clockwise
		}
		if (m == "rotateRight")
		{
			yaw(-rotateStepsize);
		}

		if (m == "rotateUp")
		{
			pitch(-rotateStepsize);		// anti clockwise
		}
		if (m == "rotateDown")
		{
			pitch(rotateStepsize);
		}
		if (m == "moveForward")
		{
			glm::vec3 v = glm::vec3(0, 0, -moveStepsize);
#if NO_ROLL
			translate(getOrientation() * v);
#else
			translate(m_orientation * v);
#endif
		}
		if (m == "moveBackward")
		{
			glm::vec3 v = glm::vec3(0, 0, moveStepsize);
#if NO_ROLL
			translate(getOrientation() * v);
#else
			translate(m_orientation * v);
#endif
		}

		if (m == "moveUp")
		{
			glm::vec3 v = glm::vec3(0, moveStepsize, 0); //smaller amount here
#if NO_ROLL
			translate(getOrientation() * v);
#else
			translate(m_orientation * v);
#endif
		}
		if (m == "moveDown")
		{
			glm::vec3 v = glm::vec3(0, -moveStepsize, 0);
#if NO_ROLL
			translate(getOrientation() * v);
#else
			translate(m_orientation * v);
#endif
		}
		if (m == "moveLeft")
		{
			glm::vec3 v = glm::vec3(-moveStepsize, 0, 0); 
			translate(getOrientation() * v);
		}
		if (m == "moveRight")
		{
			glm::vec3 v = glm::vec3(moveStepsize, 0, 0); 
			translate(getOrientation() * v);
		}

		if (m == "reset")
		{
			m_position = glm::vec3(0, 0, 0);
#if NO_ROLL
			m_orientationYaw = glm::quat(1, 0, 0, 0);
			m_orientationPitch = glm::quat(1, 0, 0, 0);
#else
			m_orientation = glm::quat(1, 0, 0, 0);
#endif
		}

	}

#if NO_ROLL
	TransformComponent() : m_position(0), m_orientationYaw(1, 0, 0, 0), m_orientationPitch(1,0,0,0), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos) : m_position(pos), m_orientationYaw(1, 0, 0, 0), m_orientationPitch(1,0,0,0), m_scale(1.0f) {}
	//TransformComponent(const glm::vec3& pos, const glm::quat& orient) : m_position(pos), m_orientationYaw(orient), m_scale(1.0f) {}
	//TransformComponent(const glm::vec3& pos, const glm::quat& orient, const glm::vec3& scale) : m_position(pos), m_orientation(orient), m_scale(scale) {}
	
	
	TransformComponent(const glm::vec3& pos, const glm::quat& orientY) : m_position(pos), m_orientationYaw(orientY), m_orientationPitch(1,0,0,0), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos, const glm::quat& orientY, const glm::quat& orientP) : m_position(pos), m_orientationYaw(orientY), m_orientationPitch(orientP), m_scale(1.0f) {}
#else
	TransformComponent() : m_position(0), m_orientation(1,0,0,0), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos) : m_position(pos), m_orientation(1, 0, 0, 0), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos, const glm::quat& orient) : m_position(pos), m_orientation(orient), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos, const glm::quat& orient, const glm::vec3& scale) : m_position(pos), m_orientation(orient), m_scale(scale) {}
#endif
	const glm::vec3& position() const { return m_position; }
#if NO_ROLL
#else
	const glm::quat& orientation() const { return m_orientation; }
#endif
	const glm::vec3& scale() const { return m_scale; }

	glm::mat4 getModelMatrix()
	{
		glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), m_position);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
#if NO_ROLL
		glm::mat4 rotMatrix = glm::mat4_cast(getOrientation());
#else
		glm::mat4 rotMatrix = glm::mat4_cast(m_orientation);
#endif
		return transMatrix * rotMatrix * scaleMatrix;
	}

	void translate(const glm::vec3 &v) { m_position += v; }
	void translate(float x, float y, float z) { m_position += glm::vec3(x, y, z); }

	// this is not used in steves pitch roll yaw calls
//	void rotate(float angle, const glm::vec3 &axis) 
//	{ 
//#if NO_ROLL
//#else
//		m_orientation *= glm::angleAxis(angle, m_orientation * axis); 
//#endif
//	}

#if NO_ROLL
#else
	void rotate(float angle, float x, float y, float z) {
		// show lecture
#if 1
		glm::vec3 anglesBefore = glm::eulerAngles(m_orientation); //pitch yaw roll

		// look at this
		m_orientation *= glm::angleAxis(angle, glm::vec3(x, y, z)); // *m_orientation);  //order altered - put back - PC makes no odds

		glm::vec3 anglesAfter = glm::eulerAngles(m_orientation); //pitch yaw roll

#else
		// this is steve's implementation
		m_orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orientation);  //order altered - put back - PC makes no odds
#endif
	}
#endif
	void scaleUp(const glm::vec3 &v) { m_scale += v; }
	void scaleUp(float x, float y, float z) { m_scale += glm::vec3(x, y, z); }

#if NO_ROLL
	void yaw(float angle)
	{
		m_orientationYaw *= glm::angleAxis(angle, glm::vec3(0, 1, 0));
	}
	void pitch(float angle)
	{
		m_orientationPitch *= glm::angleAxis(angle, glm::vec3(1, 0, 0));
	}
#else
	void yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f); }
	void pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f); }
	void roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f); }
#endif
	// used to show on display...
	glm::vec3 getEulerAngles()
	{
#if NO_ROLL
		glm::quat m_orientation =  m_orientationYaw * m_orientationPitch;
#endif
		return glm::eulerAngles(m_orientation); //pitch yaw roll
	}
};
