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

const int WIDTH = 800;
const int HEIGHT = 600;


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
	
	glfwSetWindowTitle("render2texture");

	glfwEnable(GLFW_STICKY_KEYS);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	GLuint programId = LoadProgram("render2texture.vs", "render2texture.fs");

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


	///////////////////////////////////////
	GLuint frameBufferName = 0;
	glGenFramebuffersEXT(1, &frameBufferName);
	glBindFramebufferEXT(GL_FRAMEBUFFER, frameBufferName);

	GLuint render2Texture;
	glGenTextures(1, &render2Texture);
	glBindTexture(GL_TEXTURE_2D, render2Texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/*
	if(!GLEW_ARB_framebuffer_object)
	{
		P("framebuffer object not supported");
		return -1;
	}
	*/

	GLuint depthBuffer;
	glGenRenderbuffersEXT(1, &depthBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER, depthBuffer);

	glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render2Texture, 0);

	GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawBuffers);

	if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		P("frame buffer not complete");
		return -1;
	}

	static const GLfloat screenQuad[] = {
											-1.0f, -1.0f, 0.0f,
											 1.0f, -1.0f, 0.0f,
											 -1.0f, 1.0f, 0.0f,
											 -1.0f, 1.0f, 0.0f,
											 1.0f, -1.0f, 0.0f,
											 1.0f, 1.0f, 0.0f
										};
	GLuint screenQuadVertexBuffer;
	glGenBuffers(1, &screenQuadVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuad), screenQuad, GL_STATIC_DRAW);

	GLuint quadProgramId = LoadProgram("passthrough.vs", "passthrough.fs");
	GLuint quadVertexPosition = glGetAttribLocation(quadProgramId, "vertexPosition");
	GLuint quadTextureId = glGetUniformLocation(quadProgramId, "texture");
	GLuint quadTimeId = glGetUniformLocation(quadProgramId, "time");

	while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED))
	{

		glBindFramebufferEXT(GL_FRAMEBUFFER, frameBufferName);
		glViewport(0, 0, WIDTH, HEIGHT);

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

		///////////////////////////////////////////////
		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WIDTH, HEIGHT);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(quadProgramId);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render2Texture);
		glUniform1i(quadTextureId, 0);

		glUniform1f(quadTimeId, (float)(glfwGetTime()*10.0f));

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, screenQuadVertexBuffer);
		glVertexAttribPointer(quadVertexPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers();
	}

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &texturePositionBuffer);

	glDeleteProgram(programId);
	glDeleteTextures(1, &textureId);

	glfwTerminate();
	return 0;
}
