#include "DynamicEnviromentObject.h"

#include "TransformComponent.h"
#include "ModelComponent.h"



DynamicEnvironmentObject::DynamicEnvironmentObject(Model* model, glm::vec3 position, glm::quat orientation)
{
	addComponent(new TransformComponent(position, orientation));
	addComponent(new ModelComponent(model));
}

DynamicEnvironmentObject::~DynamicEnvironmentObject()
{
}

void DynamicEnvironmentObject::OnUpdate(float dt)
{
	glm::vec3 rotationVector(1.0f, 0, 0); // Vector of object rotation
	TransformComponent* transform = this->getComponent<TransformComponent>(); // Get a pointer to the transform component
	//angle += dt/5000;
	transform->yaw(dt); // Rotate the tranform component based on the rotation vector and delta time
}
void DynamicEnvironmentObject::OnMessage(const std::string msg)
{
	// nothing yet
}