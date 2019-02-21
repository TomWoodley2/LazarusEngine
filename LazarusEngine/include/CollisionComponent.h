#pragma once

#include <iostream>
#include <glad/glad.h> 
#include "Component.h"

class CollisionComponent : public Component
{
private:
	glm::vec3 m_position;

public:
	CollisionComponent(const glm::vec3& pos) : m_position(pos)
	{
	}

	void OnUpdate(float dt) override
	{
		
	}

	void OnMessage(const std::string m) override
	{

	}

};

