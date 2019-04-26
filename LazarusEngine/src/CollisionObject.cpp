#include "CollisionObject.h"

#include "TransformComponent.h"
#include "ModelComponent.h"
#include "RigidbodyComponent.h"

CollisionObject::CollisionObject(Model* model, glm::vec3 position, glm::quat orientation)
{
	addComponent(new TransformComponent(position, orientation));
	addComponent(new ModelComponent(model));
	addComponent(new RigidbodyComponent(position,orientation, glm::vec3(1.0f,1.0f,0.0f))); // With hard coded velocity value
}

CollisionObject::~CollisionObject()
{
}

void CollisionObject::OnUpdate(float dt)
{
	getComponent<RigidbodyComponent>()->OnUpdate(dt);
	TransformComponent* tc = getComponent<TransformComponent>();

	glm::vec3 velocity = getComponent<RigidbodyComponent>()->getVelocity();
	tc->translate(velocity * dt);

	//this->getComponent<RigidbodyComponent>()->OnUpdate(dt); // Update the collision component based on dt

	// Update transform component based on rigidbodyComponent
}
void CollisionObject::OnMessage(const std::string msg)
{
	
}