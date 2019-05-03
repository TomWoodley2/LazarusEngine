#include <Windows.h>
#include "ExampleGame.h"

#include "ModelManager.h"
#include "Scene.h"

//needed for test code
#include "GameObject.h"
#include "TransformComponent.h"
#include "SceneStateComponent.h"

//#include <stdio.h>
#include <string>
//#include <vector>
//#include <iostream>
//#include <fstream>
#include <sstream>
#include "InputHandler.h"


ExampleGame::ExampleGame(IEngineCore* engine) : Game(engine)
{
	Initialise();
	//int desiredSceneIndex = m_scene->getPlayer()->getComponent<SceneStateComponent>()->SetSceneIndex(0);
}

void ExampleGame::update(float dt) 
{
	m_scene->update(dt,m_engineInterfacePtr);

	int desiredSceneIndex = m_scene->getPlayer()->getComponent<SceneStateComponent>()->GetSceneIndex();

	if (desiredSceneIndex != m_sceneIndex)
	{
		delete m_scene;

		m_sceneIndex = desiredSceneIndex;

		m_scene = new Scene(m_levelNames[m_sceneIndex], m_theModelManager, m_engineInterfacePtr);
		m_inputHandler = new InputHandler(m_scene->getPlayer());  // or have a set function perhaps better then a new instance!
	}
}

void ExampleGame::render() 
{
	m_scene->render(m_engineInterfacePtr);

	double frameDuration = m_engineInterfacePtr->getFrameDuration();

	SHORT keyState = GetKeyState(VK_TAB);
	bool isToggled = keyState & 1;

	m_scene->setDebugMode(isToggled); // Change the state in scene class

	if (isToggled != false && frameDuration > 0)
	{
		glm::vec3 eulerAngles = m_scene->getPlayer()->getEulerAngles();

		// convert to deg
		eulerAngles.x = glm::degrees(eulerAngles.x);
		eulerAngles.y = glm::degrees(eulerAngles.y);
		eulerAngles.z = glm::degrees(eulerAngles.z);
		std::ostringstream oss;
		std::ostringstream oss1;
		std::ostringstream oss2;
		std::ostringstream oss3;
		std::ostringstream oss4;
		std::ostringstream oss5;
		std::ostringstream oss6;
		std::ostringstream oss7;
		std::ostringstream oss8;
		std::ostringstream oss9;
		oss << " (" << eulerAngles.x << ", " << eulerAngles.y << ", " << eulerAngles.z << ")";
		oss1 << "Controls:";
		oss2 << "Tab = Toggle debug mode";
		oss3 << "WASD = Movement";
		oss4 << "IJKL/LMB = Rotate";
		oss5 << "1,2 = Camera";
		oss6 << "9,0 = Level";
		oss7 << "Q = Fire";
		oss8 << "fps:" << (1 / frameDuration);
		oss9 << "Fire at the moving black box to get points!";

		//oss << frameDuration;
		//renderText([string stream name].str(), [Xpos(0-1)], [Ypos(0-1)], [Text Size], glm::vec3[Text Colour(R, G, B)]
		m_engineInterfacePtr->renderText(oss.str(), 0.01f, 0.01f, 1, glm::vec3(1, 1, 0));
		m_engineInterfacePtr->renderText(oss1.str(), 0.01f, 0.96f, 0.50f, glm::vec3(1, 1, 0));
		m_engineInterfacePtr->renderText(oss2.str(), 0.01f, 0.92f, 0.50f, glm::vec3(1, 1, 0));
		m_engineInterfacePtr->renderText(oss3.str(), 0.01f, 0.88f, 0.50f, glm::vec3(1, 1, 0));
		m_engineInterfacePtr->renderText(oss4.str(), 0.01f, 0.84f, 0.50f, glm::vec3(1, 1, 0));
		m_engineInterfacePtr->renderText(oss5.str(), 0.01f, 0.80f, 0.50f, glm::vec3(1, 1, 0));
		m_engineInterfacePtr->renderText(oss6.str(), 0.01f, 0.76f, 0.50f, glm::vec3(1, 1, 0));
		m_engineInterfacePtr->renderText(oss7.str(), 0.01f, 0.72f, 0.50f, glm::vec3(1, 1, 0));
		m_engineInterfacePtr->renderText(oss8.str(), 0.01f, 0.68f, 0.50f, glm::vec3(1, 1, 0));
		m_engineInterfacePtr->renderText(oss9.str(), 0.40f, 0.01f, 1.f, glm::vec3(0, 1, 0));
	}
}

//bool ExampleGame::loadLevelJSON(std::string levelJSONFile)
//{
//
//
//}

void ExampleGame::Initialise()
{
	m_theModelManager = new ModelManager();	// singleton later...
	m_scene = new Scene(m_levelNames[m_sceneIndex], m_theModelManager, m_engineInterfacePtr);
	m_inputHandler = new InputHandler(m_scene->getPlayer());
}

/*class ShowControls : public InputCommand
{
	void execute();
};*/