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
#include "TriggerObject.h"
#include "CollisionObject.h"
#include "RigidbodyComponent.h"
#include "Rigidbody.h"

#include <fstream>
#include <sstream>
#include <math.h>

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

	m_Wander = new wander;

	engineCore->getMouseState(m_mouseX, m_mouseY, m_mouseButtons);

	m_oldMouseX = m_mouseX;
	m_oldMouseY = m_mouseY;
	m_oldMouseButtons = m_mouseButtons;
}


Scene::~Scene()
{

}


void Scene::checkStaticDynamicCollisions()
{
	// Checking for collisions between all dynamic objects and all static objects

	if (staticCollisionPositions.size() != 0 && dynamicCollisionPositions.size() != 0)
	{
		for (int j = 0; j < staticCollisionPositions.size(); j++)
		{
			glm::vec3 staticCNegative = v_gameObjects[staticCollisionPositions[j]]->getComponent<TransformComponent>()->position() + v_gameObjects[staticCollisionPositions[j]]->getComponent<ModelComponent>()->getModel()->getNegativeCorner();
			glm::vec3 staticCPositive = v_gameObjects[staticCollisionPositions[j]]->getComponent<TransformComponent>()->position() + v_gameObjects[staticCollisionPositions[j]]->getComponent<ModelComponent>()->getModel()->getPositiveCorner();

			for (int i = 0; i < dynamicCollisionPositions.size(); i++)
			{
				glm::vec3 dynamicCNegative = v_gameObjects[dynamicCollisionPositions[i]]->getComponent<TransformComponent>()->position() + v_gameObjects[dynamicCollisionPositions[i]]->getComponent<ModelComponent>()->getModel()->getNegativeCorner();
				glm::vec3 dynamicCPositive = v_gameObjects[dynamicCollisionPositions[i]]->getComponent<TransformComponent>()->position() + v_gameObjects[dynamicCollisionPositions[i]]->getComponent<ModelComponent>()->getModel()->getPositiveCorner();


				RigidbodyComponent *const  dynamicBody = v_gameObjects[dynamicCollisionPositions[i]]->getComponent<RigidbodyComponent>();

				if (m_collision.checkAABBCollision(staticCNegative, staticCPositive, dynamicCNegative, dynamicCPositive))
				{
					if (hasStoppedColliding[i + (j*(dynamicCollisionPositions.size()))])
					{

						//std::cout << "Static-Dynamic Collision " << std::endl;
						RigidbodyComponent *const  staticBody = v_gameObjects[staticCollisionPositions[j]]->getComponent<RigidbodyComponent>();

						// If object was a trigger
						if (v_gameObjects[staticCollisionPositions[j]]->getObjectType() == "Trigger")
						{
							playerScore++;
						}

						// These all hard coded for y values -> for full collision, will need to swap the velocity based on the force

						if (m_collision.getClosestPlane() == 'X')
						{

							dynamicBody->setVelocity(glm::vec3(m_collision.getPlaneValue() * dynamicBody->getVelocity().x * dynamicBody->getBounceCoefficient() * staticBody->getBounceCoefficient(), dynamicBody->getVelocity().y, dynamicBody->getVelocity().z));


						}
						else if (m_collision.getClosestPlane() == 'Y')
						{

							dynamicBody->setVelocity(glm::vec3(dynamicBody->getVelocity().x, m_collision.getPlaneValue() *  dynamicBody->getVelocity().y * dynamicBody->getBounceCoefficient() * staticBody->getBounceCoefficient(), dynamicBody->getVelocity().z));
						}
						else if (m_collision.getClosestPlane() == 'Z')
						{


							dynamicBody->setVelocity(glm::vec3(dynamicBody->getVelocity().x, dynamicBody->getVelocity().y, m_collision.getPlaneValue() * dynamicBody->getVelocity().z * staticBody->getBounceCoefficient() * dynamicBody->getBounceCoefficient()));



						}
						else
						{
							std::cout << "error" << std::endl;
						}



						std::cout << "Velocity : X : " << dynamicBody->getVelocity().x << " Y : " << dynamicBody->getVelocity().y << " Z : " << dynamicBody->getVelocity().z << std::endl;



						hasStoppedColliding[i + (j*(dynamicCollisionPositions.size()))] = false;
					}
					else
					{

						// Sliding here
						
						if (dynamicBody->getVelocity().y < 0.3f && dynamicBody->getVelocity().y > -0.3f)
						{
							dynamicBody->setVelocity(glm::vec3(dynamicBody->getVelocity().x * 0.2, 0, dynamicBody->getVelocity().z * 0.2));
							dynamicBody->setForce(glm::vec3(0.0, 0, 0.0));
							dynamicBody->setAcceleration(glm::vec3(0.0, 0, 0.0));
						}
						

					}
				}
				else
				{
					hasStoppedColliding[i + (j*(dynamicCollisionPositions.size()))] = true;
				}
			}

		}
	}
}

void Scene::checkDynamicDynamicCollisions()
{
	if (dynamicCollisionPositions.size() != 0)
	{
		int k = 0; // Used to count place is hasStoppedColliding

		for (int i = 0; i < dynamicCollisionPositions.size() - 1; i++)
		{
			glm::vec3 d1CNegative = v_gameObjects[dynamicCollisionPositions[i]]->getComponent<TransformComponent>()->position() + v_gameObjects[dynamicCollisionPositions[i]]->getComponent<ModelComponent>()->getModel()->getNegativeCorner();
			glm::vec3 d1CPositive = v_gameObjects[dynamicCollisionPositions[i]]->getComponent<TransformComponent>()->position() + v_gameObjects[dynamicCollisionPositions[i]]->getComponent<ModelComponent>()->getModel()->getPositiveCorner();

			for (int j = i + 1; j < dynamicCollisionPositions.size(); j++)
			{
				//std::cout << "i : " << i << " j : " << j << std::endl;

				glm::vec3 d2CNegative = v_gameObjects[dynamicCollisionPositions[j]]->getComponent<TransformComponent>()->position() + v_gameObjects[dynamicCollisionPositions[j]]->getComponent<ModelComponent>()->getModel()->getNegativeCorner();
				glm::vec3 d2CPositive = v_gameObjects[dynamicCollisionPositions[j]]->getComponent<TransformComponent>()->position() + v_gameObjects[dynamicCollisionPositions[j]]->getComponent<ModelComponent>()->getModel()->getPositiveCorner();

				if (m_collision.checkAABBCollision(d1CNegative, d1CPositive, d2CNegative, d2CPositive))
				{
					if (hasStoppedCollidingDD[k])
					{
						std::cout << "Dynamic-Dynamic Collision" << std::endl;
						// Get rididbodies for each dynamic object
						RigidbodyComponent *const  d1Body = v_gameObjects[dynamicCollisionPositions[i]]->getComponent<RigidbodyComponent>();
						RigidbodyComponent *const  d2Body = v_gameObjects[dynamicCollisionPositions[j]]->getComponent<RigidbodyComponent>();

						if (m_collision.getClosestPlane() == 'X')
						{
							d1Body->setVelocity(glm::vec3(m_collision.getPlaneValue() * d1Body->getVelocity().x * d2Body->getBounceCoefficient() * d1Body->getBounceCoefficient(), d1Body->getVelocity().y, d1Body->getVelocity().z));
							d2Body->setVelocity(glm::vec3(m_collision.getPlaneValue() * d2Body->getVelocity().x * d2Body->getBounceCoefficient() * d1Body->getBounceCoefficient(), d2Body->getVelocity().y, d2Body->getVelocity().z));
						}
						else if (m_collision.getClosestPlane() == 'Y')
						{
							d1Body->setVelocity(glm::vec3(d1Body->getVelocity().x, m_collision.getPlaneValue() * d1Body->getVelocity().y * d2Body->getBounceCoefficient() * d1Body->getBounceCoefficient(), d1Body->getVelocity().z));
							d2Body->setVelocity(glm::vec3(d2Body->getVelocity().x, m_collision.getPlaneValue() * d2Body->getVelocity().y * d2Body->getBounceCoefficient() * d1Body->getBounceCoefficient(), d2Body->getVelocity().z));
						}
						else if (m_collision.getClosestPlane() == 'Z')
						{
							d1Body->setVelocity(glm::vec3(d1Body->getVelocity().x, d1Body->getVelocity().y, m_collision.getPlaneValue() * d1Body->getVelocity().z * d1Body->getBounceCoefficient() * d2Body->getBounceCoefficient()));
							d2Body->setVelocity(glm::vec3(d2Body->getVelocity().x, d2Body->getVelocity().y, m_collision.getPlaneValue() * d2Body->getVelocity().z * d1Body->getBounceCoefficient() * d2Body->getBounceCoefficient()));
						}
						else
						{
							std::cout << "error" << std::endl;
						}



						hasStoppedCollidingDD[k] = false;
					}
				}
				else
				{
					hasStoppedCollidingDD[k] = true;
					//std::cout << "No dynamic Collision" << std::endl;
				}
				//std::cout << "K : " << k << std::endl;
				k++;

			}
		}
	}

}


void Scene::update(float dt,IEngineCore* engineCore)
{
	// Check if should move to next scene
	PlayerCharacter* playerCharacter = getPlayer();
	if (playerScore >= maxPlayerScore)
	{
		playerCharacter->getComponent<SceneStateComponent>()->SetSceneIndex(playerCharacter->getComponent<SceneStateComponent>()->GetSceneIndex()+1);
		//playerScore = 0;
		return;
	}
	
	m_Wander->move();

	checkStaticDynamicCollisions(); // Check for collisions between static and dynamic objects
	checkDynamicDynamicCollisions(); // Check for collisions between all dynamic objects

	// Update dynamic objects based on dt
	for (int i = 0; i < v_gameObjects.size(); i++)
	{
		v_gameObjects[i]->OnUpdate(dt);
	}

	// Create a new object when fire button is pressed
	if (playerCharacter->getComponent<FireObjectComponent>()->GetShouldFire() == true)
	{
		
		
		GameObject * m_gameObject;
		Model * m_model;

		m_model = m_theModelManager->getModel("assets/models/orangeSphere.obj");


		
		// Set position based on player position
		glm::vec3 m_position(playerCharacter->getComponent<TransformComponent>()->position());
		glm::quat orientation(1.0f, 0.0f, 0.0f, 0.0f);

		// Create a collision object
		m_gameObject = new CollisionObject(m_model, m_position, orientation);
		m_gameObject->setObjectType("DynamicCollision");
		//std::cout << "Fire!" << std::endl;

		// Attempt to rotate velocity based on player rotation
		glm::quat playerOrientation = playerCharacter->getComponent<TransformComponent>()->getOrientation();
		glm::vec3 playerEulerOrientation =  (glm::eulerAngles(playerOrientation) );
		
		//std::cout << "playerEulerOrientation " << playerEulerOrientation.x << "," << playerEulerOrientation.y << "," << playerEulerOrientation.z << std::endl;

		//glm::mat3 xRotated = 
		// Not sure about this
		float newY = playerEulerOrientation.y + ((30 * 3.14159 / 180));

		float xVal = (12.f * cos(newY)) - (12.f * sin(newY));
		float yVal = 12.f;
		float zVal = (12.f * sin(newY)) + (12.f *cos(newY));

		 
		// Setup rigidbody
		Rigidbody m_rigidbody;
		m_rigidbody.bounceCoefficient = 0.6f;
		m_rigidbody.gravityEnabled = true;
		m_rigidbody.mass = 1.0f;
		m_rigidbody.velocity = glm::vec3(xVal, yVal, zVal);
		//m_rigidbody.velocity = glm::vec3(10, 5, 0);
		
		//m_gameObject->setPhysicsProperties( Use in future )
		m_gameObject->getComponent<RigidbodyComponent>()->setRigidbody(m_rigidbody);

		//std::cout << v_gameObjects.size()+1 << std::endl;

		// Add this rigid body to store of dynamicPositions
		dynamicCollisionPositions.push_back(v_gameObjects.size());

		// Push back new possible collision states for static and dynamic

		for (int i = 0; i < staticCollisionPositions.size() ; i++)
		{
			hasStoppedColliding.push_back(true);
		}

		for (int i = 0; i < dynamicCollisionPositions.size() ; i++)
		{
			hasStoppedCollidingDD.push_back(true);
		}

		// Add to game objects
		v_gameObjects.push_back(m_gameObject);
		
		//delete m_gameObject;
		//delete m_model;
		

		playerCharacter->getComponent<FireObjectComponent>()->SetShouldFire(false);

		Model* model = m_gameObject->getComponent<ModelComponent>()->getModel();
		glm::vec3 negativeMeshCorner = model->getNegativeCorner(); // Get the corner points of the model
		glm::vec3 positiveMeshCorner = model->getPositiveCorner();
		engineCore->updatePhysicsBoxVertices(negativeMeshCorner, positiveMeshCorner); 

		//fillCollidingVectors();
		// Check if required score reached
		
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

	m_Wander->RenderNav();
	
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
		else if (gameObject->getObjectType() == "Trigger")
		{
			ObjColour = glm::vec3(0.5f, 0.0f, 0.5f);

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
		engineCore->renderText("Score : " + std::to_string(playerScore), 0.80f, 0.90f, 1, glm::vec3(1, 0, 0));

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

		const Json::Value typeNode = gameObjects[i]["type"]; // Game object type
		const Json::Value physicsNode = gameObjects[i]["physics"]; // Physics information

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
		else if (typeNode == "trigger")
		{
			thisGameObject = new TriggerObject(model, position, orientation);
			thisGameObject->setObjectType("Trigger");
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
		thisGameObject->setPhysicsProperties(physicsNode.asInt()); // Set each object's physics reference

		v_gameObjects.push_back(thisGameObject);

		//delete thisGameObject;
	}

	// PHYSICS
	const Json::Value physicsProperties = root["Physics"];


	

	std::cout << "staticCollision" << std::endl;

	if (staticCollisionPositions.size() != 0)
	{
		for (int i = 0; i < staticCollisionPositions.size(); i++)
		{
			std::cout << v_gameObjects[staticCollisionPositions[i]]->getPhysicsProperties() << std::endl;

			Rigidbody m_rigidbody;

			// Velocity
			const Json::Value velocityNode = physicsProperties[v_gameObjects[staticCollisionPositions[i]]->getPhysicsProperties()]["velocity"];
			m_rigidbody.velocity = glm::vec3(velocityNode[0].asFloat(), velocityNode[1].asFloat(), velocityNode[2].asFloat());

			// Mass 
			const Json::Value massNode = physicsProperties[v_gameObjects[staticCollisionPositions[i]]->getPhysicsProperties()]["mass"];
			m_rigidbody.mass = massNode.asFloat();

			// Bounce
			const Json::Value bounceNode = physicsProperties[v_gameObjects[staticCollisionPositions[i]]->getPhysicsProperties()]["bounce"];
			m_rigidbody.bounceCoefficient = bounceNode.asFloat();

			// Gravity
			const Json::Value gravityNode = physicsProperties[v_gameObjects[staticCollisionPositions[i]]->getPhysicsProperties()]["gravity"];
			m_rigidbody.gravityEnabled = gravityNode.asBool();

			// Locked
			const Json::Value lockedNode = physicsProperties[v_gameObjects[staticCollisionPositions[i]]->getPhysicsProperties()]["locked"];
			m_rigidbody.positionLocked = lockedNode.asBool();

			v_gameObjects[staticCollisionPositions[i]]->getComponent<RigidbodyComponent>()->setRigidbody(m_rigidbody);
		}
	}

	std::cout << "dynamicCollision" << std::endl;

	if (dynamicCollisionPositions.size() != 0)
	{
		for (int i = 0; i < dynamicCollisionPositions.size(); i++)
		{
			std::cout << v_gameObjects[dynamicCollisionPositions[i]]->getPhysicsProperties() << std::endl;

			int physicsPropertiesInt = v_gameObjects[dynamicCollisionPositions[i]]->getPhysicsProperties();

			Rigidbody m_rigidbody;

			// If object has physics properties

			const Json::Value velocityNode = physicsProperties[physicsPropertiesInt]["velocity"];
			m_rigidbody.velocity = glm::vec3(velocityNode[0].asFloat(), velocityNode[1].asFloat(), velocityNode[2].asFloat());

			// Mass 
			const Json::Value massNode = physicsProperties[physicsPropertiesInt]["mass"];
			m_rigidbody.mass = massNode.asFloat();

				// Bounce
			const Json::Value bounceNode = physicsProperties[physicsPropertiesInt]["bounce"];
			m_rigidbody.bounceCoefficient = bounceNode.asFloat();

			// Gravity
			const Json::Value gravityNode = physicsProperties[physicsPropertiesInt]["gravity"];
			m_rigidbody.gravityEnabled = gravityNode.asBool();

			// Locked
			const Json::Value lockedNode = physicsProperties[physicsPropertiesInt]["locked"];
			m_rigidbody.positionLocked = lockedNode.asBool();
			
			

			v_gameObjects[dynamicCollisionPositions[i]]->getComponent<RigidbodyComponent>()->setRigidbody(m_rigidbody);

		}
		fillCollidingVectors();
		

	}
	
	
	//hasStoppedCollidingDD = true;


	return loadOK;
}

void Scene::fillCollidingVectors()
{
	hasStoppedColliding.reserve(dynamicCollisionPositions.size()*staticCollisionPositions.size());
		

	for (int i = 0; i < dynamicCollisionPositions.size()*staticCollisionPositions.size(); i++)
	{
		hasStoppedColliding.push_back(true);
			
	}

	int currentDynamicSize = dynamicCollisionPositions.size();
	int totalDynamicSize = 0;

	for (int i = dynamicCollisionPositions.size() - 1; i > 0; i--)
	{
		totalDynamicSize += i;
	}

	hasStoppedCollidingDD.reserve(totalDynamicSize);

	for (int i = 0; i < totalDynamicSize; i++)
	{
		hasStoppedCollidingDD.push_back(true);
	}
}

PlayerCharacter* Scene::getPlayer()
{
	return (PlayerCharacter*)v_gameObjects[m_playerIndex];
}


void Scene::setDebugMode(bool state)
{
	renderDebugMode = state;
}