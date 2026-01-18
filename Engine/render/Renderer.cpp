#include "Renderer.h"
#include "ShaderProgram.h"
#include "InputCallback.h"
#include "Log.h"
#include "Assets/Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

unsigned int VBO, VAO;

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
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	//window = glfwCreateWindow(mode->width, mode->height, "Engine", glfwGetPrimaryMonitor(), nullptr);
	window = glfwCreateWindow((int)(mode->width * 0.8f), (int)(mode->height * 0.8f), "Pong", nullptr, nullptr);
	Globals::SetScreenRatio((float)mode->width / (float)mode->height);

	if (window == nullptr)
	{
		LOG_FATAL("Failed to create GLFW window");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

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

	// Setting camera
	CameraMatrixLocation = CurrentShaderProgram->GetUniformLocation("CameraMatrix");
	glm::mat4 ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.0f, 10.0f));
	glm::mat4 ProjectionMatrix = glm::ortho(-Globals::GetScreenHalfWidth(), Globals::GetScreenHalfWidth(), -Globals::GetScreenHalfHeight(), Globals::GetScreenHalfHeight(), -20.f, 20.f);
	CameraMatrix = ProjectionMatrix * ViewMatrix;
	glUniformMatrix4fv(CameraMatrixLocation, 1, GL_FALSE, glm::value_ptr(CameraMatrix));

	return true;
}

void Renderer::Tick(float DeltaTime)
{
	Level* CurrentLevel = Globals::GetLevel();
	if (!CurrentLevel)
	{
		return;
	}
	glClearColor(0.05f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderModels(DeltaTime, CurrentLevel);
	
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Renderer::GetWindowShouldCLose()
{
	return glfwWindowShouldClose(window);
}

Renderer::~Renderer()
{
	glfwSetWindowShouldClose(window, GL_TRUE);
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::RenderModels(float DeltaTime, Level* CurrentLevel)
{
	const std::vector<std::shared_ptr<Actor>> ActorsOnLevel = CurrentLevel->GetActorsOnLevel();
	for (std::shared_ptr<Actor> ActorOnLevel : ActorsOnLevel)
	{
		glm::mat4 ModelMatrix = ActorOnLevel->GetModelMatrix();
		glUniformMatrix4fv(ModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

		const std::vector<std::shared_ptr<Model>> Models = ActorOnLevel->GetActorsModels();
		for (std::shared_ptr<Model> ActorsModel : Models)
		{
			AssetManager* AssetMan = Globals::GetAssetManager();
			GLuint TexAddres = AssetMan->GetTextureAddress(ActorsModel->GetTexturePath(), ActorsModel->GetTextureID());
			glBindTexture(GL_TEXTURE_2D, TexAddres);
			std::pair<GLuint, unsigned int> MeshInfo = AssetMan->GetMeshAddressAndIndicesSize(ActorsModel->GetMeshPath(), ActorsModel->GetMeshID());
			glBindVertexArray(MeshInfo.first);

			glDrawElements(AssetMan->GetMeshDrawingMode(ActorsModel->GetMeshID()), MeshInfo.second, GL_UNSIGNED_INT, 0);
		}
	}
}
