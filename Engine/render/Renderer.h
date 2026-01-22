// OpenGL Extension Wrangler Library
#include <GL/glew.h>
// Windowing library
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include <vector>
#include <memory>

class Renderer
{
public:
	Renderer() = default;
	bool InitRenderer();  

	void BeginFrame();
	void EndFrame();

	void RenderModels(const std::vector<std::shared_ptr<class Actor>>& Actors);
	void RenderTextUIs(const std::vector<std::shared_ptr<class TextUI>>& TextUIsOnLevel);

	void PollWindowEvents();
	bool GetWindowShouldCLose();

	~Renderer();

private:
	GLFWwindow* window;

	float ScreenRatio;

	// Uniforms for shaders
	GLint CameraMatrixLocation; 
	GLint ModelMatrixLocation;
	GLint BaseTexture;
	std::shared_ptr<class ShaderProgram> ModelsShaderProgram;

	// Uniforms for shaders
	GLint TextSPFontAtlasLocation;
	GLint TextSPCameraMatrixLocation;
	GLint TextSPModelMatrixLocation;
	GLint TextSPColorLocation;
	std::shared_ptr<class ShaderProgram> TextShaderProgram;

	glm::mat4 CameraMatrix;
};

