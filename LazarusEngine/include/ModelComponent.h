#pragma once
#include "Component.h"


class Model;


class ModelComponent :  public Component
{
public:
	ModelComponent(Model* model) : m_model(model)
	{
		//m_model = model;
	};

	Model* getModel()
	{
		return m_model;
	}

	void OnUpdate(float dt) override
	{

	}

	void OnMessage(const std::string m) override
	{
	}

private:

	Model* m_model;
};

