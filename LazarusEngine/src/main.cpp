#define USE_GLFW 1
#ifdef USE_GLFW
	#include "GLFW_EngineCore.h"
#endif

#include "ExampleGame.h"

int main(int argc, char* argv[])
{
	IEngineCore* engineCore;

#ifdef USE_GLFW
	engineCore = new GLFW_EngineCore;
#endif

	if (!engineCore->initWindow(1000, 800, "TransformAndData"))
		return -1;

	Game* myFirstGame = new ExampleGame(engineCore);

	engineCore->runEngine(myFirstGame);

	return 0;
}
