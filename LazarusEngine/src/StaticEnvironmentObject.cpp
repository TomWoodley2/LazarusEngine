#include "StaticEnvironmentObject.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "RigidbodyComponent.h"

StaticEnvironmentObject::StaticEnvironmentObject(Model* model, glm::vec3 position, glm::quat orientation)
{
	// Set all rigidbody properties
	m_rigidbody.mass = 1.0f;
	m_rigidbody.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_rigidbody.bounceCoefficient = 1.0f;
	m_rigidbody.gravityEnabled = false;
	m_rigidbody.positionLocked = false;

	addComponent(new TransformComponent(position, orientation));
	addComponent(new ModelComponent(model));
	addComponent(new RigidbodyComponent(position, orientation, m_rigidbody)); // Not using gravity
}

StaticEnvironmentObject::~StaticEnvironmentObject() 
{
}

void StaticEnvironmentObject::OnUpdate(float dt)
{
	// Used for any updating - none actually required here
	getComponent<RigidbodyComponent>()->OnUpdate(dt);
	TransformComponent* tc = getComponent<TransformComponent>();

	glm::vec3 velocity = getComponent<RigidbodyComponent>()->getVelocity();
	tc->translate(velocity * dt);
}
void StaticEnvironmentObject::OnMessage(const std::string msg)
{
	// nothing yet
}