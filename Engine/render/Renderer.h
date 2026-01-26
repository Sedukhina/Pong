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
	Renderer(GLFWwindow* Window);

	void BeginFrame();
	void EndFrame();

	void RenderModels(const std::vector<std::shared_ptr<class Actor>>& Actors);
	void RenderTextUIs(const std::vector<std::shared_ptr<class TextUI>>& TextUIsOnLevel);

private:
	GLFWwindow* Window = nullptr;

	float ScreenRatio = -1;

	// Uniforms for shaders
	GLint CameraMatrixLocation = -1; 
	GLint ModelMatrixLocation = -1;
	GLint BaseTexture = -1;
	std::shared_ptr<class ShaderProgram> ModelsShaderProgram = nullptr;

	// Uniforms for shaders
	GLint TextSPFontAtlasLocation = -1;
	GLint TextSPCameraMatrixLocation = -1;
	GLint TextSPModelMatrixLocation = -1;
	GLint TextSPColorLocation = -1;
	std::shared_ptr<class ShaderProgram> TextShaderProgram = nullptr;

	glm::mat4 CameraMatrix = glm::mat4(0);
};

