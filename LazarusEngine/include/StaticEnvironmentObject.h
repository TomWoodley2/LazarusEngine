#pragma once
#include "GameObject.h"
#include <glm/glm.hpp>

class Model;

class StaticEnvironmentObject : public GameObject
{
public:
	StaticEnvironmentObject(Model* model, glm::vec3 position, glm::quat orientation);
	~StaticEnvironmentObject();

	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;
};