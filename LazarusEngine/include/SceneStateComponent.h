#pragma once
#include "Component.h"

class SceneStateComponent : public Component
{
public:
	SceneStateComponent();
	~SceneStateComponent();

	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;

	int GetSceneIndex();
	void SetSceneIndex(int newSceneIndex);

private:

	int m_sceneIndex;

};