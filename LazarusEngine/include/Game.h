#pragma once
#include "IEngineCore.h"
#include "InputHandler.h"

class Model;
class Scene;

class Game
{
public:
	Game(IEngineCore* engine);
	virtual ~Game();

	virtual void update(float dt) = 0;
	virtual void render() = 0;
	
	InputHandler* getInputHandler();

protected:
	IEngineCore* m_engineInterfacePtr;
	InputHandler* m_inputHandler;
	Scene* m_scene;
};
