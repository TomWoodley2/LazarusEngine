#pragma once
#include "GameObject.h"
#include <glm/glm.hpp>

class Model;

class DynamicEnvironmentObject : public GameObject
{
public:
	DynamicEnvironmentObject(Model* model, glm::vec3 position, glm::quat orientation);
	~DynamicEnvironmentObject();

	float angle;
	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;
};