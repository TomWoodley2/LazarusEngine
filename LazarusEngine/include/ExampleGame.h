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
	bool UI = true; // Toggle debug mode

private:

	ModelManager* m_theModelManager;
	int m_sceneIndex{ 0 };
	//these could be loaded from a file...
	std::string m_levelNames[2] = {
		"assets/levels/Level1.json",
		"assets/levels/Level2.json"
	};
};
