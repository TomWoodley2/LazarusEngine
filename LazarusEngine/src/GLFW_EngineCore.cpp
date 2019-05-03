
#include "GLFW_EngineCore.h"
#include "Game.h"
#include <fstream>
#include <sstream>
#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model.h"
#include <ft2build.h>

#include FT_FREETYPE_H 


std::vector<bool> GLFW_EngineCore::m_keyBuffer;
int GLFW_EngineCore::m_screenWidth;
int GLFW_EngineCore::m_screenHeight;

double GLFW_EngineCore::m_mouseX;
double GLFW_EngineCore::m_mouseY;
int GLFW_EngineCore::m_mouseButtons;



GLFW_EngineCore::~GLFW_EngineCore()
{
	// cleanup
	glfwTerminate();
}

bool GLFW_EngineCore::initWindow(int width, int height, std::string windowName)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	m_screenWidth = width;
	m_screenHeight = height;

	m_window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	if (m_window == nullptr)
	{
		std::cout << "Failed to create GLFW m_window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		return false;
	}

	// callback functions
	glfwSetFramebufferSizeCallback(m_window, windowResizeCallbackEvent);
	glfwSetKeyCallback(m_window, keyCallbackEvent);


	glfwSetMouseButtonCallback(m_window, mouseButtonCallbackEvent);
	glfwSetCursorPosCallback(m_window, mouseMoveCallbackEvent);

	// make space for the keybuffer
	m_keyBuffer.resize(m_keyBufferSize);
	std::fill(m_keyBuffer.begin(), m_keyBuffer.end(), false);

	// set the shaders to the given default ones
	setDefaultShaders();
	setupDefaultFont();
	
	// Setup the rendering for the physics box
	//initPhysicsBox();

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	// enable alpha transparency
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

bool GLFW_EngineCore::runEngine(Game* game)			// was Game&
{
	// for this example just give the game direct access to the engine
	// there are other ways this could be handled
	// game.m_engineInterfacePtr = this;

	// prepare the array for frame times
	int currentFrame = 0;
	m_frameTimes.resize(m_framesToMonitor);
	std::fill(m_frameTimes.begin(), m_frameTimes.end(), 0); // Fill frame times with 0's
 
	// message loop
	while (!glfwWindowShouldClose(m_window))
	{
		std::chrono::high_resolution_clock::time_point frameStartTime = std::chrono::high_resolution_clock::now(); // Start of frame
	
		// check if we have set up any input handling
		game->getInputHandler()->handleInputs(m_keyBuffer);

		game->update(m_frameTimes[currentFrame]); // update game logic based on the time of this frame
		game->render(); // prepare game to send info to the renderer in engine core

		// swap buffers
		glfwSwapBuffers(m_window);
		glfwPollEvents();

		std::chrono::high_resolution_clock::time_point frameEndTime = std::chrono::high_resolution_clock::now(); // End of frame
		std::chrono::duration<double> frameDuration = std::chrono::duration_cast<std::chrono::duration<double>>(frameEndTime - frameStartTime); // Duration of frame

		// Update frame in the vector and change current frame
		m_frameTimes[currentFrame] = frameDuration.count(); 
		currentFrame = (currentFrame + 1) % m_framesToMonitor;
		//std::cout << m_frameTimes[currentFrame] << std::endl;

		
	}

	return true;
}

double GLFW_EngineCore::getFrameDuration()
{
	double total = 0;
	for (auto time : m_frameTimes)
	{
		total += time;
	}
	return total/m_frameTimes.size();
}

void GLFW_EngineCore::renderColouredBackground(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// screen is between 0 and 1 with (0,0) at the bottom left corner
// typical use: m_engineInterfacePtr->renderText("Awesome Game Debug Mode", 0.05f, 0.05f, 1.0f, glm::vec3(0.9, 0.9f, 0.1f));
void GLFW_EngineCore::renderText(std::string text, float x, float y, float scale, glm::vec3 colour)
{
	// set the window to orthographic
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_screenWidth), 0.0f, static_cast<float>(m_screenHeight));
	
	float pixelValueX = x * m_screenWidth;
	float pixelValueY = y * m_screenHeight;

	glUseProgram(m_fontShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(m_fontShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// Activate corresponding render state	
	glUniform3f(glGetUniformLocation(m_fontShaderProgram, "textColour"), colour.x, colour.y, colour.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(font_VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = pixelValueX + ch.bearing.x * scale;
		GLfloat ypos = pixelValueY - (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, font_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		pixelValueX += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// go back to default shader
	glUseProgram(m_defaultShaderProgram);
}


void GLFW_EngineCore::setCamera(const CameraComponent* cam)
{
	// set the view and projection components of our shader to the camera values
	glm::mat4 projection = glm::perspective(glm::radians(cam->m_fov), (float)m_screenWidth / (float)m_screenHeight, 0.1f, 10000.0f);
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(cam->getViewMatrix()));

	// be sure to activate shader when setting uniforms/drawing objects
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "objectColour"), 1.0f, 0.6f, 0.61f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightColour"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightPos"), 0.0f, 2.0f, -2.0f);
	glUniform3fv(glGetUniformLocation(m_defaultShaderProgram, "viewPos"), 1, glm::value_ptr(cam->position()));

	glUseProgram(m_boxShaderProgram);

	glUniformMatrix4fv(glGetUniformLocation(m_boxShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(m_boxShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(cam->getViewMatrix()));

	glUseProgram(m_defaultShaderProgram);



}

void GLFW_EngineCore::drawModel(Model* model, const glm::mat4& modelMatrix)
{
	// set the model component of our shader to the object model
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	model->render(m_defaultShaderProgram);
}

//-----------------------------Private functions------------------------------
void GLFW_EngineCore::keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UNKNOWN || key > m_keyBufferSize)
	{
		return;
	}
	m_keyBuffer[key] = ((action == GLFW_PRESS || action == GLFW_REPEAT));

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void GLFW_EngineCore::windowResizeCallbackEvent(GLFWwindow* window, int width, int height)
{
	// change the opengl viewport to match the new m_window size
	m_screenWidth = width;
	m_screenHeight = height;
	glViewport(0, 0, width, height);
}

void GLFW_EngineCore::loadShader(string vertexShaderFile, string fragmentShaderFile, GLuint& shaderProgram)
{
	// Load contents of vertex file
	std::ifstream inFile(vertexShaderFile);
	if (!inFile) {
		string errorMsg = "Error opening shader file: " + vertexShaderFile + "\n";
		fprintf(stderr, errorMsg.c_str());
		exit(1);
	}

	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	std::string codeStr(code.str());
	const GLchar* vertex_shader[] = { codeStr.c_str() };

	// Load contents of fragment file
	std::ifstream inFile2(fragmentShaderFile);
	if (!inFile2) {
		string errorMsg = "Error opening shader file: " + fragmentShaderFile + "\n";
		fprintf(stderr, errorMsg.c_str());
		exit(1);
	}

	std::stringstream code2;
	code2 << inFile2.rdbuf();
	inFile2.close();
	std::string codeStr2(code2.str());
	const GLchar* fragment_shader[] = { codeStr2.c_str() };

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vertex_shader, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragment_shader, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

// loading some default shaders to get things up and running
void GLFW_EngineCore::setDefaultShaders()
{
	loadShader("assets/shaders/defaultShader.vert", "assets/shaders/defaultShader.frag", m_defaultShaderProgram);
	loadShader("assets/shaders/fontShader.vert", "assets/shaders/fontShader.frag", m_fontShaderProgram);
	loadShader("assets/shaders/box.vert", "assets/shaders/box.frag", m_boxShaderProgram);
	
	// set the default shader
	glUseProgram(m_defaultShaderProgram);
}

// based on tutorial from https://learnopengl.com/#!In-Practice/Text-Rendering
void GLFW_EngineCore::setupDefaultFont()
{
	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "assets/fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}
	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &font_VAO);
	glGenBuffers(1, &font_VBO);
	glBindVertexArray(font_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, font_VBO);
	// dynamic draw as the text may change frequently
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GLFW_EngineCore::mouseMoveCallbackEvent(GLFWwindow* window, double xPos, double yPos)
{
	m_mouseX = -xPos;
	m_mouseY = -yPos;

}

void GLFW_EngineCore::mouseButtonCallbackEvent(GLFWwindow* window, int button, int action, int mods)
{
	//const int GLFW_RELEASE = 0;
	//const int GLFW_PRESS = 1;
	//const int GLFW_MOUSE_BUTTON_LEFT = 0;
	//const int GLFW_MOUSE_BUTTON_RIGHT = 1;

//	if (button & (1 << GLFW_MOUSE_BUTTON_LEFT))
//	{
//
//  }


	if (action == GLFW_PRESS)
	{
		(m_mouseButtons |= ((1 << button) & 0x03));			// butons 0 & 1 only here
	}
	else
	{
		(m_mouseButtons &= ((1 << button) & 0xFC));
	}
}


void GLFW_EngineCore::getMouseState(double& mouseX, double& mouseY, int& mouseButtons)
{
	// read latest mouse state...
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	mouseButtons = m_mouseButtons;	
}


void GLFW_EngineCore::initPhysicsBox()
{

	glm::vec3 centre = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 size = glm::vec3(2.f, 2.f, 2.f);

	//std::cout << "Negative: " << negativeMeshCornerIn.x << ", " << negativeMeshCornerIn.y << ", " << negativeMeshCornerIn.z << std::endl;
	//std::cout << "Positive: " << positiveMeshCornerIn.x << ", " << positiveMeshCornerIn.y << ", " << positiveMeshCornerIn.z << std::endl;

	
	// set vertices ready for debug
	float vertices[] = {
		centre.x - (size.x / 2), centre.y - (size.y / 2), centre.z - (size.z / 2),
		centre.x - (size.x / 2), centre.y + (size.y / 2), centre.z - (size.z / 2),
		centre.x + (size.x / 2), centre.y + (size.y / 2), centre.z - (size.z / 2),
		centre.x + (size.x / 2), centre.y - (size.y / 2), centre.z - (size.z / 2),

		centre.x - (size.x / 2), centre.y - (size.y / 2), centre.z + (size.z / 2),
		centre.x - (size.x / 2), centre.y + (size.y / 2), centre.z + (size.z / 2),
		centre.x + (size.x / 2), centre.y + (size.y / 2), centre.z + (size.z / 2),
		centre.x + (size.x / 2), centre.y - (size.y / 2), centre.z + (size.z / 2)
	};
	
	
	/*
	
	// set vertices ready for debug
	float vertices[] = {
		negativeMeshCornerIn.x, negativeMeshCornerIn.y, negativeMeshCornerIn.z,
		negativeMeshCornerIn.x, positiveMeshCornerIn.y, negativeMeshCornerIn.z,
		positiveMeshCornerIn.x, positiveMeshCornerIn.y, negativeMeshCornerIn.z,
		positiveMeshCornerIn.x, negativeMeshCornerIn.y, negativeMeshCornerIn.z,

		negativeMeshCornerIn.x, negativeMeshCornerIn.y, positiveMeshCornerIn.z,
		negativeMeshCornerIn.x, positiveMeshCornerIn.y, positiveMeshCornerIn.z,
		positiveMeshCornerIn.x, positiveMeshCornerIn.y, positiveMeshCornerIn.z,
		positiveMeshCornerIn.x, negativeMeshCornerIn.y, positiveMeshCornerIn.z
	};
	
	*/

	// Index data
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3,
		1, 5, 6,
		1, 6, 2,
		0, 4, 7,
		0, 7, 3,
		4, 5, 1,
		4, 1, 0,
		3, 2, 6,
		3, 6, 7,
		7, 6, 5,
		7, 5, 4
	};

	// Buffer and Array initialization
	glGenBuffers(1, &physics_EBO);
	glGenBuffers(1, &physics_VBO);
	glGenVertexArrays(1, &physics_VAO);

	// This is vertex array as the one in use
	glBindVertexArray(physics_VAO);

	// Bind the buffers to the array
	glBindBuffer(GL_ARRAY_BUFFER, physics_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, physics_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Setup pointers for shader to vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	

	
}

void GLFW_EngineCore::updatePhysicsBoxVertices(glm::vec3 negativeMeshCornerIn, glm::vec3 positiveMeshCornerIn)
{
	// Update which vertices are being used based on updated positions of the physics box
	std::vector<float> vertices = {
		negativeMeshCornerIn.x, negativeMeshCornerIn.y, negativeMeshCornerIn.z,
		negativeMeshCornerIn.x, positiveMeshCornerIn.y, negativeMeshCornerIn.z,
		positiveMeshCornerIn.x, positiveMeshCornerIn.y, negativeMeshCornerIn.z,
		positiveMeshCornerIn.x, negativeMeshCornerIn.y, negativeMeshCornerIn.z,

		negativeMeshCornerIn.x, negativeMeshCornerIn.y, positiveMeshCornerIn.z,
		negativeMeshCornerIn.x, positiveMeshCornerIn.y, positiveMeshCornerIn.z,
		positiveMeshCornerIn.x, positiveMeshCornerIn.y, positiveMeshCornerIn.z,
		positiveMeshCornerIn.x, negativeMeshCornerIn.y, positiveMeshCornerIn.z
	};

	v_allObjectCollisionVertices.push_back(vertices);
}



void GLFW_EngineCore::drawPhysicsBox(const glm::mat4& modelIn, int modelNo, glm::vec3 colourIn)
{
	glUseProgram(m_boxShaderProgram); // Use the box shader program

	glUniformMatrix4fv(glGetUniformLocation(m_boxShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelIn)); // send model matrix to shader
	glUniform3f(glGetUniformLocation(m_boxShaderProgram, "colour"), colourIn.x, colourIn.y, colourIn.z);
	
	//std::cout << modelNo << std::endl;

	glBindVertexArray(physics_VAO); // get the vertex array for drawing
	//glBufferData(GL_ARRAY_BUFFER, v_allObjectCollisionVertices[modelNo].size() * sizeof(float), &v_allObjectCollisionVertices[modelNo][0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, physics_VBO);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), &v_allObjectCollisionVertices[modelNo][0], GL_STATIC_DRAW);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Draw as a line
	glDrawElements(GL_TRIANGLES, 36 * sizeof(unsigned int), GL_UNSIGNED_INT, 0); // size of gets the number of points required for the number of triangles to be drawn
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // change back to filling objects
																				 
	glUseProgram(m_defaultShaderProgram); // change back to default shader

}

void GLFW_EngineCore::clearPhysicsBoxes()
{
	physics_VBOs.clear();
	v_allObjectCollisionVertices.clear();
}