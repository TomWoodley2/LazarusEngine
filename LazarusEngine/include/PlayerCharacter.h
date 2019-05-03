#pragma once
#include "GameObject.h"
#include "ColourComponent.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "SceneStateComponent.h"
#include "FireObjectComponent.h"

class PlayerCharacter : public GameObject
{
	enum class CameraViewState { firstPersonCamera, thirdPersonCamera };

public:

	// constructor
	PlayerCharacter(Model* model, glm::vec3 position, glm::quat orientation);
	~PlayerCharacter();
	
	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;

	void SetCameraPositionFromTransformComponent(TransformComponent* tc);
	
	glm::vec3 getEulerAngles();


private:

	CameraViewState m_cameraState{ CameraViewState::thirdPersonCamera }; // Change the camera view state
};

