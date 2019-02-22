#pragma once
#include "Game.h"

class ModelManager;

class ExampleGame : public Game
{
public:
	ExampleGame(IEngineCore* engine);
	void update(float dt) override;
	void render() override;
	void Initialise();

private:

	ModelManager* m_theModelManager;
	int m_sceneIndex{ 0 };
	//these could be loaded from a file...
	std::string m_levelNames[2] = {
		"assets/levels/testLevel.json",
		"assets/levels/testLevel2.json"
	};
};
