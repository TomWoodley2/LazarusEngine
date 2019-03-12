#pragma once
#include "GameObject.h"

class BackgroundColourGameObject : public GameObject
{
public:
	// has no model or transform
	BackgroundColourGameObject();
	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;
	

};
