#pragma once
#include "Component.h"

class ColourComponent : public Component
{
public:
	ColourComponent() : m_colourValue(0.25) {};

	void OnUpdate(float dt) override
	{

	}

	void OnMessage(const std::string m) override
	{
		if (m == "increase")
		{
			m_colourValue += ColourAdjustmentValue;
		}
		else if (m == "decrease")
		{
			m_colourValue -= ColourAdjustmentValue;
		}
	}
	float m_colourValue;

private:
	const float ColourAdjustmentValue = 0.001f;
	
};

class RedComponent : public ColourComponent {};
class GreenComponent : public ColourComponent {};
class BlueComponent : public ColourComponent {};