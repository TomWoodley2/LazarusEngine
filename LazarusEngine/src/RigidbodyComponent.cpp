#include "RigidbodyComponent.h"

void RigidbodyComponent::OnUpdate(float dt)
{
	m_velocity += (m_acceleration * dt);
}

void RigidbodyComponent::OnMessage(const std::string m)
{

}
