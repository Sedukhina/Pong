// OpenGL Extension Wrangler Library
#include <GL/glew.h>
// Windowing library
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include <memory>

class Renderer
{
public:
	Renderer();
	bool InitRenderer();  
	
	void Tick(float DeltaTime);
	bool GetWindowShouldCLose();

	~Renderer();

private:
	void RenderModels(float DeltaTime, class Level* CurrentLevel);
	void RenderTextUIs(float DeltaTime, class Level* CurrentLevel);

	GLFWwindow* window;

	float ScreenRatio;

	// Uniforms for shaders
	GLint CameraMatrixLocation; 
	GLint ModelMatrixLocation;
	GLint BaseTexture;
	std::unique_ptr<class ShaderProgram> ModelsShaderProgram;

	// Uniforms for shaders
	GLint TextSPFontAtlasLocation;
	GLint TextSPCameraMatrixLocation;
	GLint TextSPModelMatrixLocation;
	GLint TextSPColorLocation;
	std::unique_ptr<class ShaderProgram> TextShaderProgram;

	glm::mat4 CameraMatrix;
};

