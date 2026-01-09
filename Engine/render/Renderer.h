// OpenGL Extension Wrangler Library
#include <GL/glew.h>
// Windowing library
#include <GLFW/glfw3.h>

class Renderer
{
public:
	Renderer();
	bool InitRenderer();

	// Returns ShouldShutdown status  
	bool Tick();

	~Renderer();

private:
	GLFWwindow* window;

	float ScreenRatio;

	// Uniforms for shaders
	GLint BaseTexture;

	class ShaderProgram* CurrentShaderProgram;
};

