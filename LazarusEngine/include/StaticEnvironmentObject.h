#pragma once
#include "GameObject.h"
#include <glm/glm.hpp>

#include "Rigidbody.h"

class Model;

class StaticEnvironmentObject : public GameObject
{
private:
	Rigidbody m_rigidbody;
public:
	StaticEnvironmentObject(Model* model, glm::vec3 position, glm::quat orientation);
	~StaticEnvironmentObject();

	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;
};