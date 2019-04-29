#include "Scene.h"

#include "IEngineCore.h"
#include "Model.h"
#include "Game.h"
#include "TransformComponent.h"
#include "ColourComponent.h"
#include "ModelComponent.h"
#include "CameraComponent.h"
#include "ModelManager.h"
#include "JSON\json.h"

//#include "PlayerCharacter.h" already included in Scene.h
#include "StaticEnvironmentObject.h"
#include "DynamicEnviromentObject.h"
#include "BackgroundColourGameObject.h"
#include "CollisionObject.h"
#include "RigidbodyComponent.h"
#include "Rigidbody.h"

#include <fstream>
#include <sstream>

Scene::Scene(std::string filename, ModelManager* theModelManager, IEngineCore* engineCore) : m_theModelManager(theModelManager)
{
	m_playerBackground = new BackgroundColourGameObject();
	m_levelLoaded = loadLevelJSON(filename);

	engineCore->clearPhysicsBoxes();
	engineCore->initPhysicsBox();
	
	// Initiate the physics box for each gameObject
	for (auto gameObject : v_gameObjects)
	{
		Model* model = gameObject->getComponent<ModelComponent>()->getModel();
		glm::vec3 negativeMeshCorner = model->getNegativeCorner(); // Get the corner points of the model
		glm::vec3 positiveMeshCorner = model->getPositiveCorner();
		engineCore->updatePhysicsBoxVertices(negativeMeshCorner, positiveMeshCorner); // Update the physics box based on the model position
	}
	

	engineCore->getMouseState(m_mouseX, m_mouseY, m_mouseButtons);

	m_oldMouseX = m_mouseX;
	m_oldMouseY = m_mouseY;
	m_oldMouseButtons = m_mouseButtons;
}


Scene::~Scene()
{

}


void Scene::update(float dt)
{
	// Checking for collisions between all dynamic objects and all static objects

	for (int j = 0; j < staticCollisionPositions.size(); j++)
	{
		glm::vec3 staticCNegative = v_gameObjects[staticCollisionPositions[j]]->getComponent<TransformComponent>()->position() + v_gameObjects[staticCollisionPositions[j]]->getComponent<ModelComponent>()->getModel()->getNegativeCorner();
		glm::vec3 staticCPositive = v_gameObjects[staticCollisionPositions[j]]->getComponent<TransformComponent>()->position() + v_gameObjects[staticCollisionPositions[j]]->getComponent<ModelComponent>()->getModel()->getPositiveCorner();

		for (int i = 0; i < dynamicCollisionPositions.size(); i++)
		{
			glm::vec3 dynamicCNegative = v_gameObjects[dynamicCollisionPositions[i]]->getComponent<TransformComponent>()->position() + v_gameObjects[dynamicCollisionPositions[i]]->getComponent<ModelComponent>()->getModel()->getNegativeCorner();
			glm::vec3 dynamicCPositive = v_gameObjects[dynamicCollisionPositions[i]]->getComponent<TransformComponent>()->position() + v_gameObjects[dynamicCollisionPositions[i]]->getComponent<ModelComponent>()->getModel()->getPositiveCorner();

			if (m_collision.checkAABBCollision(staticCNegative, staticCPositive, dynamicCNegative, dynamicCPositive))
			{
				if (hasStoppedColliding[i+(j*(dynamicCollisionPositions.size()))])
				{
					RigidbodyComponent *const  staticBody = v_gameObjects[staticCollisionPositions[j]]->getComponent<RigidbodyComponent>();
					RigidbodyComponent *const  dynamicBody = v_gameObjects[dynamicCollisionPositions[i]]->getComponent<RigidbodyComponent>();
					// These all hard coded for y values -> for full collision, will need to swap the velocity based on the force

					// Get momentum and compare for each axis (momentum = mass * velocity)
					glm::vec3 dynamicMomentum = dynamicBody->getMass() * dynamicBody->getVelocity();
					glm::vec3 staticMomentum = staticBody->getMass() * staticBody->getVelocity();

					// Dynamic body velocity after
					dynamicBody->setVelocity(glm::vec3(dynamicBody->getVelocity().x, -dynamicBody->getVelocity().y * staticBody->getBounceCoefficient() * dynamicBody->getBounceCoefficient(), dynamicBody->getVelocity().z));
					
					// Therefore static body velocity after
					//glm::vec3 staticVelocityAfter = -(dynamicBody->getVelocity() * dynamicBody->getMass()) / staticBody->getMass();
					//staticBody->setVelocity(staticVelocityAfter);


					/*
					if (dynamicBody->getVelocity().y < 0.1f && dynamicBody->getVelocity().y > -0.1f)
					{
					dynamicBody->setForce(glm::vec3(0.0f, 0.0f, 0.0f));
					}
					*/
					hasStoppedColliding[i+(j*(dynamicCollisionPositions.size()))] = false;
				}
			}
			else
			{
				hasStoppedColliding[i+(j*(dynamicCollisionPositions.size()))] = true;
			}
		}

	}
	
	// Checking for collisions between dynamic objects
	// Hard coded for the 2 dynamic objects to start with

	glm::vec3 d1CNegative = v_gameObjects[dynamicCollisionPositions[0]]->getComponent<TransformComponent>()->position() + v_gameObjects[dynamicCollisionPositions[0]]->getComponent<ModelComponent>()->getModel()->getNegativeCorner();
	glm::vec3 d1CPositive = v_gameObjects[dynamicCollisionPositions[0]]->getComponent<TransformComponent>()->position() + v_gameObjects[dynamicCollisionPositions[0]]->getComponent<ModelComponent>()->getModel()->getPositiveCorner();

	glm::vec3 d2CNegative = v_gameObjects[dynamicCollisionPositions[1]]->getComponent<TransformComponent>()->position() + v_gameObjects[dynamicCollisionPositions[1]]->getComponent<ModelComponent>()->getModel()->getNegativeCorner();
	glm::vec3 d2CPositive = v_gameObjects[dynamicCollisionPositions[1]]->getComponent<TransformComponent>()->position() + v_gameObjects[dynamicCollisionPositions[1]]->getComponent<ModelComponent>()->getModel()->getPositiveCorner();

	if (m_collision.checkAABBCollision(d1CNegative, d2CPositive, d1CNegative, d2CPositive))
	{
		if (hasStoppedCollidingDD)
		{
			//std::cout << "Dynamic Collision" << std::endl;
			RigidbodyComponent *const  d1Body = v_gameObjects[dynamicCollisionPositions[0]]->getComponent<RigidbodyComponent>();
			RigidbodyComponent *const  d2Body = v_gameObjects[dynamicCollisionPositions[1]]->getComponent<RigidbodyComponent>();

			glm::vec3 d1Momentum = d1Body->getMass() * d1Body->getVelocity();
			glm::vec3 d2Momentum = d2Body->getMass() * d2Body->getVelocity();

			// Normalized velocity
			//glm::vec3 normalizedVelD1 =  glm::normalize(d1Body->getVelocity());
			//glm::vec3 normalizedVelD2 =  glm::normalize(d2Body->getVelocity());


			
			//glm::vec3 resultMomentumD1 = (normalizedVelD2 - normalizedVelD1) * d1Momentum * d1Body->getBounceCoefficient();
			//glm::vec3 resultMomentumD2 = (normalizedVelD1 - normalizedVelD2) * d2Momentum * d2Body->getBounceCoefficient();
			
			//std::cout << "XD1: " << resultMomentumD1.x << "YD1: " << resultMomentumD1.y << "ZD1: " << resultMomentumD1.z << std::endl;
			//std::cout << "XD2: " << resultMomentumD2.x << "YD2: " << resultMomentumD2.y << "ZD2: " << resultMomentumD2.z << std::endl;

			d1Body->setVelocity(glm::vec3(d1Body->getVelocity().x, -d1Body->getVelocity().y * d2Body->getBounceCoefficient() * d1Body->getBounceCoefficient(), d1Body->getVelocity().z));
			//d1Body->setVelocity(resultMomentumD1 / d1Body->getMass());
			//d2Body->setVelocity(resultMomentumD2 / d2Body->getMass());

			glm::vec3 staticVelocityAfter = -(d1Body->getVelocity() * d1Body->getMass()) / d2Body->getMass();
			d2Body->setVelocity(staticVelocityAfter);

			hasStoppedCollidingDD = false;


			
		}
		
		
	}
	else
	{
		hasStoppedCollidingDD = true;
		//std::cout << "No dynamic Collision" << std::endl;
	}
	


	// (remember to delete pointers)

	// Update dynamic objects based on dt
	for (int i = 0; i < v_gameObjects.size(); i++)
	{
		v_gameObjects[i]->OnUpdate(dt);
	}

}

void Scene::render(IEngineCore* engineCore)
{
	// try mouse code here?

	PlayerCharacter* playerCharacter = getPlayer();

	
	// keep the old mouse position and button state
	m_oldMouseX = m_mouseX;
	m_oldMouseY = m_mouseY;
	m_oldMouseButtons = m_mouseButtons;

	// get the current mouse pos and button state
	engineCore->getMouseState(m_mouseX, m_mouseY, m_mouseButtons);
	
	double xDelta = m_mouseX - m_oldMouseX;
	double yDelta = m_mouseY - m_oldMouseY;

	// do we have a new button press
	if ((m_mouseButtons & 0x1) & !(m_oldMouseButtons & 0x1))
	{
		m_mouseEnabled = !m_mouseEnabled;
	}


	if (m_mouseEnabled)
	{
		const float mouseSensitivity = 200.0f;	// related to scrfeenwidth later might be better!

		float theta = static_cast<float>(xDelta) / mouseSensitivity;
		float thi = static_cast<float>(yDelta) / mouseSensitivity;		// use screen width later...

		TransformComponent* transformComponent = playerCharacter->getComponent<TransformComponent>();

		// apply the transforms seperately to avoid roll
		transformComponent->pitch(thi);
		transformComponent->yaw(theta);

		playerCharacter->SetCameraPositionFromTransformComponent(transformComponent);
	}


	// do open gl setup for the frame (renderColourBackground is not the most informative name)
	float redValue = 0, greenValue = 0, blueValue = 1;
	if (m_playerBackground->getComponent<RedComponent>())
		redValue = m_playerBackground->getComponent<RedComponent>()->m_colourValue;
	if (m_playerBackground->getComponent<GreenComponent>())
		greenValue = m_playerBackground->getComponent<GreenComponent>()->m_colourValue;
	if (m_playerBackground->getComponent<BlueComponent>())
		blueValue = m_playerBackground->getComponent<BlueComponent>()->m_colourValue;

	engineCore->renderColouredBackground(redValue, greenValue, blueValue);

	// update the camera
	engineCore->setCamera(getPlayer()->getComponent<CameraComponent>());

	// draw the game objects
	int i = 0;
	for (auto gameObject : v_gameObjects)
	{
		Model* model = gameObject->getComponent<ModelComponent>()->getModel();
		//glm::vec3 negativeMeshCorner = model->getNegativeCorner(); // Get the corner points of the model
		//glm::vec3 positiveMeshCorner = model->getPositiveCorner();

		//std::cout << "Negative: " << negativeMeshCorner.x << ", " << negativeMeshCorner.y << ", " << negativeMeshCorner.z << std::endl;
		//::cout << "Positive: " << positiveMeshCorner.x << ", " << positiveMeshCorner.y << ", " << positiveMeshCorner.z << std::endl;

		//glm::vec3 position = gameObject->getComponent<TransformComponent>()->m_position;
		glm::mat4 matrix = gameObject->getComponent<TransformComponent>()->getModelMatrix();

		engineCore->drawModel(model, matrix);
		
		glm::vec3 ObjColour;
		if (gameObject->getObjectType() == "StaticCollision")
		{
			ObjColour = glm::vec3(0.0f, 1.0f, 0.0f);

		}
		else if (gameObject->getObjectType() == "DynamicCollision")
		{
			ObjColour = glm::vec3(0.0f, 0.0f, 1.0f);

		}
		else
		{
			ObjColour = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		// Draw the physics boxes if in debug mode
		if (renderDebugMode)
		{
			engineCore->drawPhysicsBox(matrix, i, ObjColour);
		}
		
		i++; // Used for drawing the physics boxes
		//engineCore->drawCube(matrix);

	}
}

bool Scene::loadLevelJSON(std::string levelJSONFile)
{
	bool loadOK = true;

	std::fstream jsonData;
	Json::Value root;
	Json::Reader reader;

	jsonData.open(levelJSONFile.c_str());
	// check for errors
	if (!reader.parse(jsonData, root))
	{
		std::cout << "Failed to parse data from: "
			<< levelJSONFile
			<< reader.getFormattedErrorMessages();

		loadOK = false;
		return loadOK;
	}
	const Json::Value gameObjects = root["GameObjects"];
	

	
	int numberOfCubes = gameObjects.size();
	//v_GameObjects.resize(numberOfCubes);

	// size() tells us how large the array is
	for (int i = 0; i < (int)gameObjects.size(); i++)
	{



		////////////////////////////////////////////////////////

		// get string
		std::cout << gameObjects[i]["name"].asString() << " loaded\n";




		// link this to model

		const Json::Value modelNode = gameObjects[i]["model"];
		

		std::string modelName = modelNode.asString();	// no index as not an array

		Model* model{ nullptr };

		if (modelNode.type() != Json::nullValue)
		{
			model = m_theModelManager->getModel("assets/models/" + modelName);
		}
		else
		{
			model = m_theModelManager->getModel("assets/models/cone.obj");			// change to error model later....
		}

		// test for no model later!!!!!

		if (model == nullptr)
		{
			loadOK = false;
		}

		float x, y, z;
		// get the position node
		const Json::Value posNode = gameObjects[i]["position"];
		x = posNode[0].asFloat(); // get float
		y = posNode[1].asFloat();
		z = posNode[2].asFloat();



		glm::vec3 position(x, y, z);

		glm::quat orientation(1.0f, 0.0f, 0.0f, 0.0f);


		const Json::Value orientNode = gameObjects[i]["orientation"];
		if (orientNode.type() != Json::nullValue)
		{

			// get orientation here e.t.c.

		}


		glm::vec3 scale(1.0f, 1.0f, 1.0f);
		const Json::Value scaleNode = gameObjects[i]["scale"];
		if (scaleNode.type() != Json::nullValue)
		{

			// get scale here e.t.c.

		}

		std::cout << x << "," << y << "," << z << std::endl;

		const Json::Value typeNode = gameObjects[i]["type"];

		GameObject* thisGameObject;
		if (typeNode == "player")
		{
			thisGameObject = new PlayerCharacter(model, position, orientation);
			thisGameObject->setObjectType("player");
			
			
		}
		else if (typeNode == "static")
		{


			thisGameObject = new StaticEnvironmentObject(model, position, orientation);
			thisGameObject->setObjectType("StaticCollision");
			staticCollisionPositions.push_back(i);
			
		}
		else if (typeNode == "dynamic")
		{
			thisGameObject = new DynamicEnvironmentObject(model, position, orientation);
			thisGameObject->setObjectType("dynamic");
			
		
		}
		else if (typeNode == "collision")
		{
			// Type to test collision bounding boxes
			thisGameObject = new CollisionObject(model, position, orientation);
			thisGameObject->setObjectType("DynamicCollision");
			dynamicCollisionPositions.push_back(i);


			
		}
		else
		{
			// If no type specified, set to static
			thisGameObject = new StaticEnvironmentObject(model, position, orientation);
			thisGameObject->setObjectType("static");
			
		}
		v_gameObjects.push_back(thisGameObject);

		//delete thisGameObject;
	}

	// PHYSICS
	const Json::Value physicsProperties = root["Physics"];


	

	std::cout << "staticCollision" << std::endl;
	for (int i = 0; i < staticCollisionPositions.size(); i++)
	{
		std::cout << staticCollisionPositions[i] << std::endl;

		Rigidbody m_rigidbody;

		// Velocity
		const Json::Value velocityNode = physicsProperties[staticCollisionPositions[i]]["velocity"];
		m_rigidbody.velocity = glm::vec3(velocityNode[0].asFloat(), velocityNode[1].asFloat(), velocityNode[2].asFloat());

		// Mass 
		const Json::Value massNode = physicsProperties[staticCollisionPositions[i]]["mass"];
		m_rigidbody.mass = massNode.asFloat();

		// Bounce
		const Json::Value bounceNode = physicsProperties[staticCollisionPositions[i]]["bounce"];
		m_rigidbody.bounceCoefficient = bounceNode.asFloat();

		// Gravity
		const Json::Value gravityNode = physicsProperties[staticCollisionPositions[i]]["gravity"];
		m_rigidbody.gravityEnabled = gravityNode.asBool();

		// Locked
		const Json::Value lockedNode = physicsProperties[staticCollisionPositions[i]]["locked"];
		m_rigidbody.positionLocked = lockedNode.asBool();

		v_gameObjects[staticCollisionPositions[i]]->getComponent<RigidbodyComponent>()->setRigidbody(m_rigidbody);
	}

	std::cout << "dynamicCollision" << std::endl;
	for (int i = 0; i < dynamicCollisionPositions.size(); i++)
	{
		std::cout << dynamicCollisionPositions[i] << std::endl;

		Rigidbody m_rigidbody;

		// Velocity
		const Json::Value velocityNode = physicsProperties[dynamicCollisionPositions[i]]["velocity"];
		m_rigidbody.velocity = glm::vec3(velocityNode[0].asFloat(), velocityNode[1].asFloat(), velocityNode[2].asFloat());

		// Mass 
		const Json::Value massNode = physicsProperties[dynamicCollisionPositions[i]]["mass"];
		m_rigidbody.mass = massNode.asFloat();

		// Bounce
		const Json::Value bounceNode = physicsProperties[dynamicCollisionPositions[i]]["bounce"];
		m_rigidbody.bounceCoefficient = bounceNode.asFloat();

		// Gravity
		const Json::Value gravityNode = physicsProperties[dynamicCollisionPositions[i]]["gravity"];
		m_rigidbody.gravityEnabled = gravityNode.asBool();

		// Locked
		const Json::Value lockedNode = physicsProperties[dynamicCollisionPositions[i]]["locked"];
		m_rigidbody.positionLocked = lockedNode.asBool();

		v_gameObjects[dynamicCollisionPositions[i]]->getComponent<RigidbodyComponent>()->setRigidbody(m_rigidbody);

	}
	

	hasStoppedColliding.reserve(dynamicCollisionPositions.size()*staticCollisionPositions.size());

	for (int i = 0; i < dynamicCollisionPositions.size()*staticCollisionPositions.size(); i++)
	{
		hasStoppedColliding.push_back(true);
	}
	

	hasStoppedCollidingDD = true;


	return loadOK;
}

#if 0

bool Scene::loadLevelJSONOLD(std::string levelJSONFile)
{
	std::fstream jsonData;
	Json::Value root;
	Json::Reader reader;

	jsonData.open(levelJSONFile.c_str());
	// check for errors
	if (!reader.parse(jsonData, root))
	{
		std::cout << "Failed to parse data from: "
			<< levelJSONFile
			<< reader.getFormattedErrorMessages();
		return false;
	}
	const Json::Value gameObjects = root["GameObjects"];

	//

	int numberOfCubes = gameObjects.size();
	//v_GameObjects.resize(numberOfCubes);

	// size() tells us how large the array is
	for (int i = 0; i < (int)gameObjects.size(); i++)
	{



		////////////////////////////////////////////////////////

		// get string
		std::cout << gameObjects[i]["name"].asString() << " loaded\n";
		
		
		// link this to model later....
		
		
		float x, y, z;
		// get the position node
		const Json::Value posNode = gameObjects[i]["position"];
		x = posNode[0].asFloat(); // get float
		y = posNode[1].asFloat();
		z = posNode[2].asFloat();

		glm::vec3 position(x, y, z);

		glm::quat orientation(1.0f, 0.0f, 0.0f, 0.0f);


		const Json::Value orientNode = gameObjects[i]["orientation"];
		if (orientNode.type() != Json::nullValue)
		{

			// get orientation here e.t.c.

		}


		glm::vec3 scale(1.0f, 1.0f, 1.0f);
		const Json::Value scaleNode = gameObjects[i]["scale"];
		if (scaleNode.type() != Json::nullValue)
		{

			// get scale here e.t.c.

		}
		// todo - fix this to be data dependent
		if (i == 0)
		{
			v_gameObjects.push_back(new PlayerCharacter(m_model, position, orientation));
		}
		else
		{
			v_gameObjects.push_back(new StaticEnvironmentObject(m_model, position, orientation));
		}
	}





	return true;
}

#endif

PlayerCharacter* Scene::getPlayer()
{
	return (PlayerCharacter*)v_gameObjects[m_playerIndex];
}






void Scene::loadLevel(std::string levelFile)
{

	Model* model = nullptr;;	//fix if we use this

	std::ifstream myInputFile;
	myInputFile.open(levelFile, std::ios_base::in);

	std::stringstream ss;
	std::string s;

	int numCubesToRead;

	if (myInputFile.is_open())
	{
		// read the first line which has the number of elements
		std::getline(myInputFile, s);
		ss.str(s);
		ss.ignore(17); // ignore the first 20 chars

		//std::string title;
		//ss >> title;
		ss >> numCubesToRead;

		// resize the correct size for the vector
		v_gameObjects.resize(numCubesToRead);
		ss.clear();

		for (int i = 0; i < numCubesToRead; i++)
		{
			getline(myInputFile, s);
			ss.clear();
			ss.str(s);

			// cube x - just ignore for now..

			// use a stringstream to get integer values
			getline(myInputFile, s);
			ss.clear();
			ss.str(s);

			float x;
			float y;
			float z;

			ss >> x;
			ss >> y;
			ss >> z;
			glm::vec3 position(x, y, z);

			getline(myInputFile, s);
			ss.clear();
			ss.str(s);

			float ow;
			float ox;
			float oy;
			float oz;

			ss >> ow;
			ss >> ox;
			ss >> oy;
			ss >> oz;
			glm::quat orientation(ow, ox, oy, oz);

			getline(myInputFile, s);
			ss.clear();
			ss.str(s);

			float sx;
			float sy;
			float sz;

			ss >> sx;
			ss >> sy;
			ss >> sz;

			glm::vec3 scale(sx, sy, sz);

			v_gameObjects.push_back(new StaticEnvironmentObject(model, position, orientation));
		}
	}
}

void Scene::setDebugMode(bool state)
{
	renderDebugMode = state;
}