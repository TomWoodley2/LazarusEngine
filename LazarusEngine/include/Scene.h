#pragma once

// include assimp first - error C2589 for 
#include "Model.h"

#include "GameObject.h"
#include "PlayerCharacter.h"
#include "IEngineCore.h"

#include "Collision.h" // Used for collision between objects

#include <string>


//class Model;
class ModelManager;


class Scene
{

public:

	Scene(std::string filename, ModelManager* theModelManager, IEngineCore* engineCore);
	~Scene();


	void update(float dt);
	
	

	void render(IEngineCore* engineCore);
	
	

	void loadLevel(std::string levelFile);
	bool loadLevelJSON(std::string levelJSONFile);


	PlayerCharacter* getPlayer();

	void setDebugMode(bool state); // Change state of debug mode based on game class

private:

	bool m_levelLoaded{ false };

	int m_playerIndex{ 1 };

	GameObject* m_playerBackground;


	std::vector<GameObject*> v_gameObjects;
	ModelManager* m_theModelManager;

	// mouse data - we keep old position (last frame or update) so we can get delta's

	double m_oldMouseX;
	double m_oldMouseY;
	double m_mouseX;
	double m_mouseY;
	int m_mouseButtons;
	int m_oldMouseButtons;
	bool m_mouseEnabled{ false };

	bool renderDebugMode = true;

	Collision m_collision ;

	std::vector<int> staticCollisionPositions; // Store positions of static collision objects in the GameObject array
	std::vector<int> dynamicCollisionPositions; // Store positions of dynamic collision objects in the GameObject array

	std::vector<bool> hasStoppedColliding; // Store if the object has stopped colliding ( stop multiple procs on collision )
	bool hasStoppedCollidingDD; // Used for dynamic-dynamic collision to check if collision has stopped
	
};
