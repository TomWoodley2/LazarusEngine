#pragma once
#include "GameObject.h"
#include <glm/glm.hpp>

#include "Rigidbody.h"

class Model;

class TriggerObject : public GameObject
{
private:
	Rigidbody m_rigidbody;
	glm::vec3 startPosition;
	bool movingRight = true;
public:
	TriggerObject(Model* model, glm::vec3 position, glm::quat orientation);
	~TriggerObject();

	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;
};
