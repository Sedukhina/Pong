#include "ShaderProgram.h"
#include "Log.h"
#include <fstream>
#include "Assets/Asset.h"

ShaderProgram::ShaderProgram(std::filesystem::path pathToVertex, std::filesystem::path pathToFragment)
{
	if (!IsExistingPath(&pathToVertex))
	{
		LOG_FATAL("Provided path to vertex shader doesn't exist: ");
		LOG_FATAL(pathToVertex.string().c_str());
		throw std::runtime_error("Provided path to vertex shader doesn't exist" + pathToVertex.string());
	}
	std::ifstream VertexShaderFile(pathToVertex);
	std::string VertCode((std::istreambuf_iterator<char>(VertexShaderFile)),
	std::istreambuf_iterator<char>());

	if (!IsExistingPath(&pathToFragment))
	{
		LOG_FATAL("Provided path to fragment shader doesn't exist: ");
		LOG_FATAL(pathToFragment.string().c_str());
		throw std::runtime_error("Provided path to fragment shader doesn't exist" + pathToFragment.string());
	}
	std::ifstream FragmentShaderFile(pathToFragment);
	std::string FragCode((std::istreambuf_iterator<char>(FragmentShaderFile)),
		std::istreambuf_iterator<char>());

	const GLchar* vShaderCode = VertCode.c_str();
	const GLchar* fShaderCode = FragCode.c_str();
	GLuint Vertex, Fragment;

	Vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(Vertex, 1, &vShaderCode, NULL);
	glCompileShader(Vertex);
	GLint success;
	glGetShaderiv(Vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(Vertex, 512, NULL, infoLog);
		LOG_FATAL("Vertex shader compilation error: ");
		LOG_FATAL(infoLog);
	}

	Fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(Fragment, 1, &fShaderCode, NULL);
	glCompileShader(Fragment);
	glGetShaderiv(Fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(Fragment, 512, NULL, infoLog);
		LOG_FATAL("Fragment shader compilation error: ");
		LOG_FATAL(infoLog);
	}

	this->Program = glCreateProgram();
	glAttachShader(this->Program, Vertex);
	glAttachShader(this->Program, Fragment);
	glLinkProgram(this->Program);
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(Fragment, 512, NULL, infoLog);
		LOG_FATAL("Shader program compilation error : ");
		LOG_FATAL(infoLog);
	}
	glDeleteShader(Fragment);
	glDeleteShader(Vertex);
}

void ShaderProgram::Use()
{
	glUseProgram(this->Program);
}

GLint ShaderProgram::GetUniformLocation(const char* Name)
{
	return glGetUniformLocation(Program, Name);
}