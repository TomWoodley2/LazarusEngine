
#include "FireObjectComponent.h"

#include <iostream>

FireObjectComponent::FireObjectComponent()
{
	//m_sceneIndex = 0;
}
FireObjectComponent::~FireObjectComponent()
{

}

void FireObjectComponent::OnUpdate(float dt)
{
		timeSinceTriggered += dt;
}
void FireObjectComponent::OnMessage(const std::string msg)
{
	if (msg == "FireObject")
	{
		if (timeSinceTriggered > 1.0f)
		{
			ShouldFire = true;
			timeSinceTriggered = 0;
		}
		
	}
	else
	{
		ShouldFire = false;
	}


}

bool FireObjectComponent::GetShouldFire()
{
	return ShouldFire;
}

void FireObjectComponent::SetShouldFire(bool ShouldFireIn)
{
	ShouldFire = ShouldFireIn;
}

