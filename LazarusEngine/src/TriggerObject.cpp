#include "TriggerObject.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "RigidbodyComponent.h"

TriggerObject::TriggerObject(Model* model, glm::vec3 position, glm::quat orientation)
{
	// Set all rigidbody properties

	startPosition = position;

	m_rigidbody.mass = 1.0f;
	m_rigidbody.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_rigidbody.bounceCoefficient = 1.0f;
	m_rigidbody.gravityEnabled = false;
	m_rigidbody.positionLocked = false;

	addComponent(new TransformComponent(position, orientation));
	addComponent(new ModelComponent(model));
	addComponent(new RigidbodyComponent(position, orientation, m_rigidbody)); // Not using gravity
}

TriggerObject::~TriggerObject() 
{
}

void TriggerObject::OnUpdate(float dt)
{
	// Used for any updating - none actually required here
	getComponent<RigidbodyComponent>()->OnUpdate(dt);
	TransformComponent* tc = getComponent<TransformComponent>();

	if (tc->position().z < startPosition.z + 10.f && movingRight)
	{
		tc->translate(glm::vec3(0,0,2*dt));
	}
	else
	{
		movingRight = false;
	}

	if (tc->position().z > startPosition.z - 10.f && !movingRight)
	{
		tc->translate(glm::vec3(0,0,-2*dt));
		
		
		
	}
	else
	{
		movingRight = true;
	}
	
	//glm::vec3 velocity = getComponent<RigidbodyComponent>()->getVelocity();
	//tc->translate(velocity * dt);
}
void TriggerObject::OnMessage(const std::string msg)
{
	// nothing yet
}