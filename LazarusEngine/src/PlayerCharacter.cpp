#include "PlayerCharacter.h"




	// constructor
PlayerCharacter::PlayerCharacter(Model* model, glm::vec3 position, glm::quat orientation)
	{
		TransformComponent* tc = new TransformComponent(position, orientation);

		addComponent(tc);
		addComponent(new ModelComponent(model));
		addComponent(new CameraComponent());
		addComponent(new SceneStateComponent());
		SetCameraPositionFromTransformComponent(tc);
	}
PlayerCharacter::~PlayerCharacter()
{
}

void PlayerCharacter::OnUpdate(float dt)
{
}
void PlayerCharacter::OnMessage(const std::string msg)
{

	TransformComponent* tc = getComponent<TransformComponent>();

	if (msg == "setCameraFirstPerson")
	{
		m_cameraState = CameraViewState::firstPersonCamera;
	}
	else if (msg == "setCameraThirdPerson")
	{
		m_cameraState = CameraViewState::thirdPersonCamera;
	}
	else if (msg == "Level1")
	{
		// pass message on to sceneComponent
		getComponent<SceneStateComponent>()->OnMessage("Level1");
	}
	else if (msg == "Level2")
	{
		getComponent<SceneStateComponent>()->OnMessage("Level2");

	}
	else
	{
		// pass message to transform component
		tc->OnMessage(msg);
	}
	SetCameraPositionFromTransformComponent(tc);
}

void PlayerCharacter::SetCameraPositionFromTransformComponent(TransformComponent* tc)
{
	// get resulting position
	glm::vec3 pos = tc->m_position;
	glm::quat orient = tc->getOrientation();
	// camera behind and above us OR at centre...

	glm::vec3 relativePosition;		// could use ? operator here
	if (m_cameraState == CameraViewState::thirdPersonCamera)
	{
		relativePosition = glm::vec3(0, 0.7, 6);		//above and behind
	}
	else
	{
		relativePosition = glm::vec3(0, 0.5, -0.95);	//just in front of model
	}
	pos += orient * relativePosition;

	// put camera there
	CameraComponent* cc = getComponent<CameraComponent>();
	cc->setPosition(pos);
	cc->setOrientation(orient);
}

glm::vec3 PlayerCharacter::getEulerAngles()
{
	TransformComponent* tc = getComponent<TransformComponent>();

	return tc->getEulerAngles();
}

