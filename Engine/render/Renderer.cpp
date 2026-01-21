#include "Renderer.h"
#include "Render/ShaderProgram.h"
#include "InputCallback.h"
#include "Log.h"
#include "Assets/Model.h"
#include "Assets/AssetManager.h"
#include "Scene/Level.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

bool Renderer::InitRenderer()
{
	// Window setting section
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
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

	Globals::GetAssetManager()->LoadFont();

	glActiveTexture(GL_TEXTURE0);
	// Creating Shader Program
	ModelsShaderProgram = std::make_unique<ShaderProgram>(ShaderProgram("Shaders/Shader.vert", "Shaders/Shader.frag"));
	ModelsShaderProgram->Use();
	BaseTexture = ModelsShaderProgram->GetUniformLocation("BaseColor");
	// Texture 0 will be taken as BaseColor texture
	glUniform1i(BaseTexture, 0);

	ModelMatrixLocation = ModelsShaderProgram->GetUniformLocation("ModelMatrix");

	// Setting camera
	CameraMatrixLocation = ModelsShaderProgram->GetUniformLocation("CameraMatrix");
	glm::mat4 ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.0f, 10.0f));
	glm::mat4 ProjectionMatrix = glm::ortho(-Globals::GetScreenHalfWidth(), Globals::GetScreenHalfWidth(), -Globals::GetScreenHalfHeight(), Globals::GetScreenHalfHeight(), -20.f, 20.f);
	CameraMatrix = ProjectionMatrix * ViewMatrix;
	glUniformMatrix4fv(CameraMatrixLocation, 1, GL_FALSE, glm::value_ptr(CameraMatrix));

	// Text Shader program setup
	TextShaderProgram = std::make_unique<ShaderProgram>("Shaders/TextShader.vert", "Shaders/TextShader.frag");
	TextShaderProgram->Use();
	TextSPModelMatrixLocation = TextShaderProgram->GetUniformLocation("ModelMatrix");
	TextSPColorLocation = TextShaderProgram->GetUniformLocation("Color");
	TextSPCameraMatrixLocation = TextShaderProgram->GetUniformLocation("CameraMatrix");
	glUniformMatrix4fv(TextSPCameraMatrixLocation, 1, GL_FALSE, glm::value_ptr(CameraMatrix));
	glActiveTexture(GL_TEXTURE0);
	TextSPFontAtlasLocation = TextShaderProgram->GetUniformLocation("FontAtlas");
	glUniform1i(TextSPFontAtlasLocation, 0);

	return true;
}

void Renderer::Tick(float DeltaTime)
{
	Level* CurrentLevel = Globals::GetLevel();
	if (!CurrentLevel)
	{
		return;
	}
	glClearColor(0.05f, 0.2f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderModels(DeltaTime, CurrentLevel);
	RenderTextUIs(DeltaTime, CurrentLevel);
	
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
	ModelsShaderProgram->Use();
	glActiveTexture(GL_TEXTURE0);
	const std::vector<std::shared_ptr<Actor>> ActorsOnLevel = CurrentLevel->GetActorsOnLevel();
	for (const auto& ActorOnLevel : ActorsOnLevel)
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

void Renderer::RenderTextUIs(float DeltaTime, Level* CurrentLevel)
{
	TextShaderProgram->Use();
	GLuint Atlas = Globals::GetAssetManager()->GetFontAtlas();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Atlas);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const std::vector<std::shared_ptr<TextUI>> TextUIsOnLevel = CurrentLevel->GetTextUIsOnLevel();
	for (const auto& CurTextUI : TextUIsOnLevel)
	{
		glm::mat4 ModelMatrix = CurTextUI->GetModelMatrix();
		glUniformMatrix4fv(TextSPModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		glm::vec4 Color = CurTextUI->GetColor();
		glUniform4fv(TextSPColorLocation, 1, glm::value_ptr(Color));
		GLuint VAO = CurTextUI->GetVAO();
		glBindVertexArray(VAO);
		unsigned int IndicesSize = CurTextUI->GetIndicesSize();

		glDrawElements(GL_TRIANGLES, IndicesSize, GL_UNSIGNED_INT, 0);
	}
}
