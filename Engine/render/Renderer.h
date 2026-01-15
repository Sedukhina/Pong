// OpenGL Extension Wrangler Library
#include <GL/glew.h>
// Windowing library
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

class Renderer
{
public:
	Renderer();
	bool InitRenderer();  
	
	void Tick();

	~Renderer();

private:
	GLFWwindow* window;

	float ScreenRatio;

	// Uniforms for shaders
	GLint CameraMatrixLocation; 
	GLint ModelMatrixLocation;
	GLint BaseTexture;

	class ShaderProgram* CurrentShaderProgram;

	glm::mat4 CameraMatrix;
};

