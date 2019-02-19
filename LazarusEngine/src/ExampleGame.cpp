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
}

void ExampleGame::update(float dt) 
{
	m_scene->update(dt);

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

	if (frameDuration > 0)
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
		oss << " (" << eulerAngles.x << ", " << eulerAngles.y << ", " << eulerAngles.z << ")"; // fps:" << (1 / frameDuration);
		oss1 << "Controls:";
		oss2 << "WASD = Movement";
		oss3 << "IJKL/LMB = Rotate";
		oss4 << "1,2 = Camera";
		oss5 << "9,0 = Level";
		//oss << frameDuration;
		//renderText([string stream name].str(), [Xpos(0-1)], [Ypos(0-1)], [Text Size], glm::vec3[Text Colour(R, G, B)]
		m_engineInterfacePtr->renderText(oss.str(), 0.01f, 0.01f, 1, glm::vec3(1, 1, 0));
		m_engineInterfacePtr->renderText(oss1.str(), 0.01f, 0.96f, 0.50f, glm::vec3(1, 1, 0));
		m_engineInterfacePtr->renderText(oss2.str(), 0.01f, 0.92f, 0.50f, glm::vec3(1, 1, 0));
		m_engineInterfacePtr->renderText(oss3.str(), 0.01f, 0.88f, 0.50f, glm::vec3(1, 1, 0));
		m_engineInterfacePtr->renderText(oss4.str(), 0.01f, 0.84f, 0.50f, glm::vec3(1, 1, 0));
		m_engineInterfacePtr->renderText(oss5.str(), 0.01f, 0.80f, 0.50f, glm::vec3(1, 1, 0));
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