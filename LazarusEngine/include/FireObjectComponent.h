#pragma once
#include "Component.h"

class FireObjectComponent : public Component
{
public:
	FireObjectComponent();
	~FireObjectComponent();

	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;

	bool GetShouldFire();
	void SetShouldFire(bool ShouldFireIn);

private:
	float timeSinceTriggered = 0;
	bool ShouldFire = false;

};
