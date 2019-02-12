#include "StaticEnvironmentObject.h"
#include "TransformComponent.h"
#include "ModelComponent.h"

StaticEnvironmentObject::StaticEnvironmentObject(Model* model, glm::vec3 position, glm::quat orientation)
{
	addComponent(new TransformComponent(position, orientation));
	addComponent(new ModelComponent(model));
};

StaticEnvironmentObject::~StaticEnvironmentObject() 
{
};

void StaticEnvironmentObject::OnUpdate(float dt)
{
	// nothing yet
}
void StaticEnvironmentObject::OnMessage(const std::string msg)
{
	// nothing yet
}