#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"

#include <GL/glfw.h>

#include <glm/glm.hpp>
#include "util.h"
#include "shader_loader.h"

int main(int argc, char** argv)
{
	if(!glfwInit())
	{
		P("glfw init fail\n");
		return 1;
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);
	

	if(!glfwOpenWindow(800, 600, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
	{
		P("glfw open window fail\n");
		return 1;
	}

	if(glewInit() != GLEW_OK)
	{
		P("glew init fail\n");
		return 1;
	}
	
	glfwSetWindowTitle("triangle");

	glfwEnable(GLFW_STICKY_KEYS);

	GLuint programId = LoadProgram("vertex_shader.vs", "fragment_shader.fs");

	GLuint modelPosition = glGetAttribLocation(programId, "modelPosition");

	GLfloat modelPositions[] = {
							       -1.0f, -1.0f, 0.0f,
								   	1.0f, -1.0f, 0.0f,
									0.0f,  1.0f, 0.0f
							   };

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(modelPositions), modelPositions, GL_STATIC_DRAW);

	while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED))
	{
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programId);

		glEnableVertexAttribArray(modelPosition);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		glVertexAttribPointer(modelPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(modelPosition);
		glfwSwapBuffers();
	}

	glfwTerminate();
	glDeleteBuffers(1, &vertexBuffer);
	return 0;
}
