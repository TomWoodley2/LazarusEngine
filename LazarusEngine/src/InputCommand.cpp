#include "InputCommand.h"
#include "GameObject.h"


void RotateLeftCommand::execute(GameObject& player)
{
	player.OnMessage("rotateLeft");
}


void RotateRightCommand::execute(GameObject& player)
{
	player.OnMessage("rotateRight");
}

void RotateUpCommand::execute(GameObject& player)
{
	player.OnMessage("rotateUp");
}


void RotateDownCommand::execute(GameObject& player)
{
	player.OnMessage("rotateDown");
}


void MoveForwardCommand::execute(GameObject& player)
{
	player.OnMessage("moveForward");
}



void MoveBackCommand::execute(GameObject& player)
{
	player.OnMessage("moveBackward");
}

void MoveLeftCommand::execute(GameObject& player)
{
	player.OnMessage("moveLeft");
}

void MoveRightCommand::execute(GameObject& player)
{
	player.OnMessage("moveRight");
}



void MoveUpCommand::execute(GameObject& player)
{
	player.OnMessage("moveUp");
}


void MoveDownCommand::execute(GameObject& player)
{
	player.OnMessage("moveDown");
}

void CameraFirstCommand::execute(GameObject& player)
{
	player.OnMessage("setCameraFirstPerson");
}

void CameraThirdCommand::execute(GameObject& player)
{
	player.OnMessage("setCameraThirdPerson");
}

void ResetCommand::execute(GameObject& player)
{
	player.OnMessage("reset");
}


void GotoLevel1Command::execute(GameObject& player)
{
	player.OnMessage("Level1");
}

void GotoLevel2Command::execute(GameObject& player)
{
	player.OnMessage("Level2");
}

void FireObjectCommand::execute(GameObject& player)
{
	player.OnMessage("FireObject");
}