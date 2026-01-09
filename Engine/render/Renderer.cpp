#include "Renderer.h"
#include "ShaderProgram.h"
#include "InputCallback.h"
#include "Log.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	glfwSetKeyCallback(window, key_callback);

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

	ModelMatrixLocation = CurrentShaderProgram->GetUniformLocation("ModelMatrix");
	//IMPORTANT: TEMP
	glm::mat4 ModelMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(ModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

	// Setting camera
	CameraMatrixLocation = CurrentShaderProgram->GetUniformLocation("CameraMatrix");
	glm::mat4 ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.0f, 10.0f));
	glm::mat4 ProjectionMatrix = glm::ortho(-50.f, 50.f, -50.f, 50.f, -20.f, 20.f);
	CameraMatrix = ProjectionMatrix * ViewMatrix;
	glUniformMatrix4fv(CameraMatrixLocation, 1, GL_FALSE, glm::value_ptr(CameraMatrix));


	// IMPORTANT: TEST ONLY TRIANGLE
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // левая вершина
		 0.5f, -0.5f, 0.0f, // правая вершина
		 0.0f,  0.5f, 0.0f  // верхняя вершина   
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

bool Renderer::Tick()
{
	glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// IMPORTANT: TEST ONLY TRIANGLE
	glDrawArrays(GL_TRIANGLES, 0, 3);

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