#include "Model.h"
#include "Game.h"

#include "TransformComponent.h"
#include "ColourComponent.h"
#include "ModelComponent.h"
#include "CameraComponent.h"
#include "Scene.h"
#include <string>

Game::Game(IEngineCore* engine) : m_engineInterfacePtr(engine)
{
}
Game::~Game()
{
}

InputHandler* Game::getInputHandler()
{
	return m_inputHandler;
}

