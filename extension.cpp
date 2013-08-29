#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"

#include <GL/glfw.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "util.h"
#include "shader_loader.h"
#include "texture_loader.h"
#include "inputs.h"
#include "model_loader.h"
#include "vbo_indexer.h"

void APIENTRY DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
{
	P("OpenGL Debug Output message:\n");
	if(source == GL_DEBUG_SOURCE_API_ARB)
	{
		P("Source : API;\n");
	}
	else if(source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)
	{
		P("Source : WINDOW_SYSTEM;\n");
	}
	else if(source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)
	{
		P("Source : SHADER_COMPILER;\n");
	}
	else if(source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)
	{
		P("Source : THIRD_PARTY;\n");
	}
	else if(source == GL_DEBUG_SOURCE_APPLICATION_ARB)
	{
		P("Source : APPLICATION;\n");
	}
	else if(source == GL_DEBUG_SOURCE_OTHER_ARB)
	{
		P("Source : APPLICATION;\n");
	}
	else
	{
		P("Source : UNKNOWN");
	}

	if(type == GL_DEBUG_TYPE_ERROR_ARB)
	{
		P("Type : ERROR\n");
	}
	else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)
	{
		P("Type : DEPRECATED\n");
	}
	else if(type == GL_DEBUG_TYPE_PORTABILITY_ARB)
	{
		P("Type : PORTABILITY\n");
	}
	else if(type == GL_DEBUG_TYPE_PERFORMANCE_ARB)
	{
		P("Type : PERFORMANCE\n");
	}
	else if(type == GL_DEBUG_TYPE_OTHER_ARB)
	{
		P("Type : OTHER\n");
	}
	else
	{
		P("Type : Unknown type\n");
	}

	P("Message : %s\n", message);
}


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
	
	glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	if(GLEW_AMD_seamless_cubemap_per_texture)
	{
		P("GLEW_AMD_seamless_cubemap_per_texture available\n");
	}

	if(GLEW_ARB_debug_output)
	{
		P("GLEW_ARB_debug_output available\n");
		glDebugMessageCallbackARB(&DebugOutputCallback, NULL);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	}
	else
	{
		P("GLEW_ARB_debug_output Unavailable\n");
	}

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
	
	glfwSetWindowTitle("transparency");

	glfwEnable(GLFW_STICKY_KEYS);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	GLuint programId = LoadProgram("transparency.vs", "transparency.fs");

	GLuint mvpId = glGetUniformLocation(programId, "mvp");
	GLuint worldId = glGetUniformLocation(programId, "world");
	GLuint cameraId = glGetUniformLocation(programId, "camera");
	GLuint lightPositionId = glGetUniformLocation(programId, "lightPosition");

	P("mvp=%d,world=%d,camera=%d,lightPosition=%d\n", mvpId, worldId, cameraId, lightPositionId);


	GLuint modelPosition = glGetAttribLocation(programId, "modelPosition");
	GLuint texturePosition = glGetAttribLocation(programId, "texturePosition");
	GLuint normal = glGetAttribLocation(programId, "normal");

	glm::mat4 projection = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
	glm::mat4 camera = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mvp = projection * camera * model;

	GLuint textureImage = loadDds("uvmap.DDS");

	GLuint textureId = glGetUniformLocation(programId, "textureId");

	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	bool loaded = LoadObj("suzanne.obj", vertices, uvs, normals);

	std::vector<unsigned short> indices;

	std::vector<glm::vec3> indexedVertices;
	std::vector<glm::vec2> indexedUvs;
	std::vector<glm::vec3> indexedNormals;

	indexVbo(vertices, uvs, normals, indices, indexedVertices, indexedUvs, indexedNormals);

	P("vertex count %d, index count %d\n", vertices.size(), indices.size());
	
	/*
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint texturePositionBuffer;
	glGenBuffers(1, &texturePositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texturePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size()*sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	GLuint normalBuffer;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	*/
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexedVertices.size()*sizeof(glm::vec3), &indexedVertices[0], GL_STATIC_DRAW);

	GLuint texturePositionBuffer;
	glGenBuffers(1, &texturePositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texturePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexedUvs.size()*sizeof(glm::vec2), &indexedUvs[0], GL_STATIC_DRAW);

	GLuint normalBuffer;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexedNormals.size()*sizeof(glm::vec3), &indexedNormals[0], GL_STATIC_DRAW);

	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indices.size()*sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	glUseProgram(programId);
	GLuint lightId = glGetUniformLocation(programId, "lightPosition");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED))
	{
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glUseProgram(programId);


		computeMatrixFromInput();
		camera = getCameraMatrix();
		projection = getProjectionMatrix();

		mvp = projection * camera * model;

		glUniformMatrix4fv(mvpId, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(worldId, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(cameraId, 1, GL_FALSE, &camera[0][0]);
		

		glm::vec3 lightPosition = glm::vec3(4, 4, 4);
		glUniform3f(lightId, lightPosition.x,lightPosition.y,lightPosition.z);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureImage);

		glUniform1i(textureId, 0);

		glEnableVertexAttribArray(modelPosition);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(modelPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/*
		glEnableVertexAttribArray(modelColor);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glVertexAttribPointer(modelColor, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		*/

		glEnableVertexAttribArray(texturePosition);
		glBindBuffer(GL_ARRAY_BUFFER, texturePositionBuffer);
		glVertexAttribPointer(texturePosition, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


		glEnableVertexAttribArray(normal);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
		//glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableVertexAttribArray(modelPosition);
		glDisableVertexAttribArray(texturePosition);

		glfwSwapBuffers();
	}

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &texturePositionBuffer);

	glDeleteProgram(programId);
	glDeleteTextures(1, &textureId);

	glfwTerminate();
	return 0;
}
