#include "RigidbodyComponent.h"

void RigidbodyComponent::OnUpdate(float dt)
{
	m_acceleration = m_force * m_mass;
	m_velocity += (m_acceleration * dt);
}

void RigidbodyComponent::OnMessage(const std::string m)
{

}
