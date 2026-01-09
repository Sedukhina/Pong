#include "Renderer.h"
#include "ShaderProgram.h"
#include "Log.h"

Renderer::Renderer()
{
}

bool Renderer::InitRenderer()
{
	// Window setting section
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	window = glfwCreateWindow(mode->width, mode->height, "Engine", glfwGetPrimaryMonitor(), nullptr);
	if (window == nullptr)
	{
		LOG_FATAL("Failed to create GLFW window");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// TODO: Setting Input Section
	// glfwSetKeyCallback(window, InputCallback::key_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		LOG_FATAL("Failed to initialize GLEW");
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	/*Setting Viewport size*/
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);
	ScreenRatio = (GLfloat)screenWidth / (GLfloat)screenHeight;

	// Global OpenGL Settings
	glEnable(GL_DEPTH_TEST);

	// Creating Shader Program
	CurrentShaderProgram = &ShaderProgram("/Shaders/Shader.vert", "/Shaders/Shader.frag");
	BaseTexture = CurrentShaderProgram->GetUniformLocation("BaseColor");
	// Texture 0 will be taken as BaseColor texture
	glUniform1i(BaseTexture, 0);
	CurrentShaderProgram->Use();

	return true;
}

bool Renderer::Tick()
{
	glClearColor(0.1f, 0.3f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwSwapBuffers(window);
	glfwPollEvents();

	return glfwWindowShouldClose(this->window);
}

Renderer::~Renderer()
{
	glfwSetWindowShouldClose(window, GL_TRUE);
	glfwDestroyWindow(this->window);
	glfwTerminate();
}