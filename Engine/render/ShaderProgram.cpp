#include "ShaderProgram.h"
#include "Log.h"
#include <fstream>
#include "Assets/Asset.h"

#define STRING(x) #x
#define TO_STRING(x) STRING(x)

ShaderProgram::ShaderProgram(std::filesystem::path pathToVertex, std::filesystem::path pathToFragment)
{
	if (!IsExistingPath(&pathToVertex))
	{
		LOG_FATAL("Provided path to vertex shader doesn't exist: ");
		LOG_FATAL(pathToVertex.string().c_str());
	}
	std::ifstream vShaderFile(pathToVertex);
	std::string vertCode((std::istreambuf_iterator<char>(vShaderFile)),
	std::istreambuf_iterator<char>());

	if (!IsExistingPath(&pathToFragment))
	{
		LOG_FATAL("Provided path to fragment shader doesn't exist: ");
		LOG_FATAL(pathToFragment.string().c_str());
	}
	std::ifstream fShaderFile(pathToFragment);
	std::string fragCode((std::istreambuf_iterator<char>(fShaderFile)),
		std::istreambuf_iterator<char>());

	const GLchar* vShaderCode = vertCode.c_str();
	const GLchar* fShaderCode = fragCode.c_str();
	GLuint vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	GLint success;
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		LOG_FATAL("Vertex shader compilation error: ");
		LOG_FATAL(infoLog);
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		LOG_FATAL("Fragment shader compilation error: ");
		LOG_FATAL(infoLog);
	}

	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		LOG_FATAL("Shader program compilation error : ");
		LOG_FATAL(infoLog);
	}
	glDeleteShader(fragment);
	glDeleteShader(vertex);
}

void ShaderProgram::Use()
{
	glUseProgram(this->Program);
}

GLint ShaderProgram::GetUniformLocation(const char* name)
{
	return glGetUniformLocation(Program, name);
}