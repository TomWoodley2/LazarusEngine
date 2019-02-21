#include "CollisionObject.h"

#include "TransformComponent.h"
#include "ModelComponent.h"
#include "CollisionComponent.h"

CollisionObject::CollisionObject(Model* model, glm::vec3 position, glm::quat orientation)
{
	addComponent(new TransformComponent(position, orientation));
	addComponent(new ModelComponent(model));
	addComponent(new CollisionComponent(position));
}

CollisionObject::~CollisionObject()
{
}

void CollisionObject::OnUpdate(float dt)
{
	this->getComponent<CollisionComponent>()->OnUpdate(dt); // Update the collision component based on dt
}
void CollisionObject::OnMessage(const std::string msg)
{
	
}