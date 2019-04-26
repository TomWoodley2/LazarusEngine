#pragma once

#include "GameObject.h"
#include <glm/glm.hpp>
#include "Rigidbody.h"

class Model;

class CollisionObject : public GameObject
{
private:
	Rigidbody m_rigidbody;

public:
	CollisionObject(Model* model, glm::vec3 position, glm::quat orientation);
	~CollisionObject();

	float angle;
	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;
};