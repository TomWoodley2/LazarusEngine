#pragma once
#include "IEngineCore.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/mat4x4.hpp>
#include "CameraComponent.h"
#include <map>


class Model;
class Game;

class GLFW_EngineCore : public IEngineCore
{
public:
	~GLFW_EngineCore() override;

	bool initWindow(int width, int height, std::string windowName) override;
	bool runEngine(Game* game) override;
	void renderColouredBackground(float r, float g, float b) override;
	void renderText(std::string text, float x, float y, float scale, glm::vec3 colour) override;

	void setCamera(const CameraComponent* cam) override;
	void drawModel(Model* model, const glm::mat4& modelMatrix) override;
	void drawPhysicsBox(const glm::mat4& modelIn, int modelNo, glm::vec3 colourIn) override; // Method to draw a physics box to the screen

	double getFrameDuration();

	void getMouseState(double& mouseX, double& mouseY, int& mouseButtons) override;

	void updatePhysicsBoxVertices(glm::vec3 negativeMeshCornerIn, glm::vec3 positiveMeshCornerIn); // Used to update positions of physicsbox
	void initPhysicsBox() override; // Setup physics box ready to be drawn to the screen

	void clearPhysicsBoxes() override; // Reset VBOs - used when starting a new level

private:

	std::vector<std::vector<float>> v_allObjectCollisionVertices; // stores each vector of collision vertices

	GLFWwindow* m_window;
	GLuint m_defaultShaderProgram;
	GLuint m_fontShaderProgram;
	GLuint m_boxShaderProgram;


	// added PC 
	static double m_mouseX;
	static double m_mouseY;
	static int m_mouseButtons;
	static int m_mouseAction;

	// Holds all state information relevant to a character as loaded using FreeType
	// based on https://learnopengl.com/#!In-Practice/Text-Rendering
	struct Character {
		GLuint textureID;   // ID handle of the glyph texture
		glm::ivec2 size;    // Size of glyph
		glm::ivec2 bearing;  // Offset from baseline to left/top of glyph
		GLuint advance;    // Horizontal offset to advance to next glyph
	};

	std::map<GLchar, Character> Characters;
	GLuint font_VAO, font_VBO;
	GLuint physics_VAO, physics_VBO, physics_EBO;

	std::vector<GLuint> physics_VBOs; // Vector to store the different VBO's for different objects
	

	// why are these static?
	static int m_screenWidth;
	static int m_screenHeight;
	static std::vector<bool> m_keyBuffer;
	static const int m_keyBufferSize = 400;//this could easily be smaller...

	// we average 64 frames to get a frame rate
	static const int m_framesToMonitor = 64;
	std::vector<double> m_frameTimes;		//size will be m_framesToMonitor


	// input callbacks
	static void mouseMoveCallbackEvent(GLFWwindow* window, double xPos, double yPos);
	static void mouseButtonCallbackEvent(GLFWwindow* window, int button, int action, int mods);
	static void keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void windowResizeCallbackEvent(GLFWwindow* window, int width, int height);
	
	void loadShader(std::string vertexShaderFile, std::string fragmentShaderFile, GLuint& shaderProgram);
	void setDefaultShaders();
	
	void setupDefaultFont();



};

