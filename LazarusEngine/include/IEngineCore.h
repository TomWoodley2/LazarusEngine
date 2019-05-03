#pragma once
#include <string>
#include <iostream>
#include <vector>

#include <glm/mat4x4.hpp>

// high definition timer code...
#include <chrono>

using namespace std::chrono_literals;

// use a fixed timestep of 1 / (60 fps) approx = 16 milliseconds
constexpr std::chrono::milliseconds dt(16ms);


class Model;
class CameraComponent;
class Game;
typedef unsigned int GLuint;

class IEngineCore
{
public:

	virtual ~IEngineCore() {}

	virtual bool initWindow(int width, int height, std::string windowName) = 0;
	virtual bool runEngine(Game* game) = 0;

	// some simple drawing tools - will want to move these to a proper renderer class later
	virtual void renderColouredBackground(float r, float g, float b) = 0;
	virtual void renderText(std::string text, float x, float y, float scale, glm::vec3 colour) = 0;
	virtual	void setCamera(const CameraComponent* cam) = 0;
	virtual void drawModel(Model* model, const glm::mat4& modelMatrix) = 0;
	virtual void drawPhysicsBox(const glm::mat4& modelIn, int modelNo,glm::vec3 colourIn) = 0;
	virtual void initPhysicsBox() = 0;
	virtual void updatePhysicsBoxVertices(glm::vec3 negativeMeshCornerIn, glm::vec3 positiveMeshCornerIn) = 0;

	virtual double getFrameDuration() = 0;

	// new for mouse
	virtual void getMouseState(double& mouseX, double& mouseY, int& mouseButtons) = 0;
	virtual void clearPhysicsBoxes() = 0;

};