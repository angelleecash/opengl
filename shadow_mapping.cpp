#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"

#include <GL/glfw.h>
#include <GL/glu.h>

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

static GLuint frameBufferName = 0;
static GLuint depthProgramId = -1;
static GLuint depthModelPosition;
//static GLuint depthTexturePosition;
//static GLuint depthIndexBuffer;
static GLuint depthTexture;
static std::vector<unsigned short> depthIndices;
static GLuint depthMvpId;
static GLuint vertexBuffer;
static GLuint texturePositionBuffer;
static GLuint normalBuffer;
static GLuint indexBuffer;
static GLuint textureImage;


#define CheckError() {\
	GLenum error = glGetError();\
	if(error != GL_NO_ERROR)\
	{\
		P("---------------------error %d %d %s\n", error, __LINE__, gluErrorString(error));\
	}\
}

static void PrepareShadowMap()
{
	depthProgramId = LoadProgram("shadow_depth.vs", "shadow_depth.fs");

	depthMvpId = glGetUniformLocation(depthProgramId, "mvp");

	textureImage = loadDds("uvmap.DDS");

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	LoadObj("room_thickwalls.obj", vertices, uvs, normals);

	std::vector<unsigned short>& indices = depthIndices;

	std::vector<glm::vec3> indexedVertices;
	std::vector<glm::vec2> indexedUvs;
	std::vector<glm::vec3> indexedNormals;

	indexVbo(vertices, uvs, normals, indices, indexedVertices, indexedUvs, indexedNormals);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexedVertices.size()*sizeof(glm::vec3), &indexedVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &texturePositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texturePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexedUvs.size()*sizeof(glm::vec2), &indexedUvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexedNormals.size()*sizeof(glm::vec3), &indexedNormals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indices.size()*sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	///////////////////////////////////////
	glGenFramebuffersEXT(1, &frameBufferName);
	glBindFramebufferEXT(GL_FRAMEBUFFER, frameBufferName);

	CheckError();
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	CheckError();

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	CheckError();
	/*
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	*/

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	GLuint renderBuffer;
	CheckError();
	glGenRenderbuffersEXT(1, &renderBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER, renderBuffer);
	CheckError();
	glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);	
	CheckError();
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

	CheckError();

	glDrawBuffer(GL_NONE);
	CheckError();
	glReadBuffer(GL_NONE);
	CheckError();
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		P("________________Frame buffer error %d\n", status);
	}
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

	PrepareShadowMap();

	GLuint programId = LoadProgram("shadow_mapping.vs", "shadow_mapping.fs");

	GLuint mvpId = glGetUniformLocation(programId, "mvp");
	GLuint worldId = glGetUniformLocation(programId, "world");
	GLuint cameraId = glGetUniformLocation(programId, "camera");
	GLuint lightInvId = glGetUniformLocation(programId, "lightDirectionInvWorld");
	GLuint depthBiasId = glGetUniformLocation(programId, "depthMvp");

	GLuint modelPosition = glGetAttribLocation(programId, "modelPosition");
	GLuint texturePosition = glGetAttribLocation(programId, "texturePosition");
	GLuint normal = glGetAttribLocation(programId, "normal");
	GLuint textureId = glGetUniformLocation(programId, "textureId");
	GLuint shadowMapId = glGetUniformLocation(programId, "shadowMap");
	GLuint lightPositionInWorld = glGetUniformLocation(programId, "lightPositionInWorld");

	while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED))
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER, frameBufferName);
		glViewport(0, 0, WIDTH, HEIGHT);

		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glUseProgram(depthProgramId);

		glm::vec3 lightInvDir = glm::vec3(0.5f, 2, 2);

		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
		glm::mat4 depthCameraMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 depthModelMatrix = glm::mat4(1.0);
		glm::mat4 depthMvpMatrix = depthProjectionMatrix * depthCameraMatrix * depthModelMatrix;

		glUniformMatrix4fv(depthMvpId, 1, GL_FALSE, &depthMvpMatrix[0][0]);

		glEnableVertexAttribArray(depthModelPosition);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(depthModelPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		//fill in depth data
		glDrawElements(GL_TRIANGLES, depthIndices.size(), GL_UNSIGNED_SHORT, (void*)0);

		CheckError();
		///////////////////////////////////////////////
		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WIDTH, HEIGHT);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glUseProgram(programId);

		computeMatrixFromInput();

		glm::mat4 projectionMatrix = getProjectionMatrix();
		glm::mat4 cameraMatrix = getCameraMatrix();
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		glm::mat4 depthMvp = projectionMatrix * cameraMatrix * modelMatrix;

		glm::mat4 baisMatrix(
					0.5, 0.0, 0.0, 0.0,
					0.0, 0.5, 0.0, 0.0,
					0.0, 0.0, 0.5, 0.0,
					0.5, 0.5, 0.5, 1.0
				);

		glm::mat4 depthBaisMvp = baisMatrix * depthMvp;

		glUniformMatrix4fv(mvpId, 1, GL_FALSE, &depthMvp[0][0]);
		glUniformMatrix4fv(worldId, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(cameraId, 1, GL_FALSE, &cameraMatrix[0][0]);
		glUniformMatrix4fv(depthBiasId, 1, GL_FALSE, &depthBaisMvp[0][0]);

		glUniform3f(lightInvId, lightInvDir.x, lightInvDir.y, lightInvDir.z);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureImage);
		glUniform1i(textureId, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glUniform1i(shadowMapId, 1);

		glEnableVertexAttribArray(modelPosition);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(modelPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(texturePosition);
		glBindBuffer(GL_ARRAY_BUFFER, texturePositionBuffer);
		glVertexAttribPointer(texturePosition, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(normal);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glUniform3f(lightPositionInWorld, lightInvDir.x, lightInvDir.y, lightInvDir.z);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		glDrawElements(GL_TRIANGLES, depthIndices.size(), GL_UNSIGNED_SHORT, (void*)0);

		CheckError();
		glfwSwapBuffers();
	}

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &texturePositionBuffer);

	glDeleteProgram(programId);
	glDeleteTextures(1, &textureId);

	glfwTerminate();
	return 0;
}
