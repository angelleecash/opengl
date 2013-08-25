#include <string>
#include <iostream>
#include <fstream>

using namespace std;
#include "util.h"
#include "shader_loader.h"

int LoadFile(const char* path, string& content)
{
	ifstream ifs(path, ios::in);
	if(!ifs.is_open())
	{
		return 1;
	}

	string line;
	while(getline(ifs, line))
	{
		content += line + "\n";
	}
	ifs.close();
	return 0;
}

int LoadShader(GLuint shaderId, const char* path)
{
	string shaderContent;

	LoadFile(path, shaderContent);
	char const* shaderContentPointer = shaderContent.c_str();
	glShaderSource(shaderId, 1, &shaderContentPointer, NULL);
	glCompileShader(shaderId);

	GLint result;
	int logLength;

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);

	//P("shader compile result %d, log length %d\n", result, logLength);
	if(logLength > 0)
	{
		char log[logLength+1];
		log[logLength] = '\0';
		glGetShaderInfoLog(shaderId, logLength, NULL, log);
		P("shader log:%s\n", log);
	}

	return 0;
}

GLuint LoadProgram(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	LoadShader(vertexShaderId, vertexShaderFile);

	LoadShader(fragmentShaderId, fragmentShaderFile);

	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	glLinkProgram(programId);

	int result;
	int logLength;
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);

	//P("link program result %d, log length %d\n", result, logLength);

	if(logLength > 0)
	{
		char log[logLength+1];
		log[logLength] = '\0';
		glGetProgramInfoLog(programId, logLength, NULL, log);
		P("link program log:\n %s\n", log);
	}

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}
