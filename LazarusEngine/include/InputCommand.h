#pragma once

// forward declaration - better than including the .h file if we don't need internal details
class GameObject;

class InputCommand
{
public:
	virtual ~InputCommand() {}
	virtual void execute(GameObject& player) = 0;
};

// classes implementing the InputCommand interface (implementations are in the .cpp file)
class RotateLeftCommand : public InputCommand
{
	void execute(GameObject& player);
};

class RotateRightCommand : public InputCommand
{
	void execute(GameObject& player);
};

class RotateUpCommand : public InputCommand
{
	void execute(GameObject& player);
};

class RotateDownCommand : public InputCommand
{
	void execute(GameObject& player);
};

class MoveForwardCommand : public InputCommand
{
	void execute(GameObject& player);
};

class MoveBackCommand : public InputCommand
{
	void execute(GameObject& player);
};

class MoveLeftCommand : public InputCommand
{
	void execute(GameObject& player);
};

class MoveRightCommand : public InputCommand
{
	void execute(GameObject& player);
};


class MoveUpCommand : public InputCommand
{
	void execute(GameObject& player);
};
class MoveDownCommand : public InputCommand
{
	void execute(GameObject& player);
};

class CameraFirstCommand : public InputCommand
{
	void execute(GameObject& player);
};
class CameraThirdCommand : public InputCommand
{
	void execute(GameObject& player);
};
class ResetCommand : public InputCommand
{
	void execute(GameObject& player);
};

class GotoLevel1Command : public InputCommand
{
	void execute(GameObject& player);
};

class GotoLevel2Command : public InputCommand
{
	void execute(GameObject& player);
};

class FireObjectCommand : public InputCommand
{
	void execute(GameObject& player);
};