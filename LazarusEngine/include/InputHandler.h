#pragma once
#include <map>
#include <vector>
#include "InputCommand.h"

class GameObject;

// much better here as a class, not a struct
class InputHandler
{	
private:

	GameObject* m_playerCube;
	std::map<int, InputCommand*> m_controlMapping;

public:

	InputHandler(GameObject* playerCube);	// : m_playerCube(playerCube);
	void handleInputs(const std::vector<bool>& keyBuffer);
};
