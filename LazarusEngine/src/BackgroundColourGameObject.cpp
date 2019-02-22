#pragma once

#include "Component.h"
#include "ColourComponent.h"
#include "BackgroundColourGameObject.h"

BackgroundColourGameObject::BackgroundColourGameObject()
{
	addComponent(new RedComponent());
	addComponent(new GreenComponent());
	addComponent(new BlueComponent());
}

// needed for the input command interface...
void BackgroundColourGameObject::OnUpdate(float dt)
{
}
void BackgroundColourGameObject::OnMessage(const std::string msg)	
{
}

