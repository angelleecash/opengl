#include "texture_loader.h"
#include "shader_loader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util.h"
#include <string.h>
#include <vector>

static GLuint textureId;
static GLuint vertexBufferId;
static GLuint uvBufferId;
static GLuint programId;
static GLuint positionId;
static GLuint uvId;
static GLuint textureImageId;

void InitText2d(const char* path)
{
	textureId =	LoadTga(path);

	glGenBuffers(1, &vertexBufferId);
	glGenBuffers(1, &uvBufferId);
	
	programId = LoadProgram("text2d.vs", "text2d.fs");

	positionId = glGetAttribLocation(programId, "position");
	uvId = glGetAttribLocation(programId, "uv");

	textureImageId = glGetUniformLocation(programId, "textureImage");

	P("text texture id=%d, programId=%d, vertex buffer id=%d, uv buffer id=%d, position id=%d, uv id=%d, texture image id=%d\n", textureId, programId, vertexBufferId, uvBufferId, positionId, uvId, textureImageId);
}
void DrawText(const char* text, int x, int y)
{
	unsigned int length = strlen(text);

	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> uvs;
	int size = 32;
	for(unsigned int i=0; i < length; i++)
	{
		glm::vec2 leftTop(x+i*size, y+size);
		glm::vec2 rightTop(x+i*size+size, y+size);
		glm::vec2 rightBottom(x+i*size+size, y);
		glm::vec2 leftBottom(x+i*size, y);

		vertices.push_back(leftTop);
		vertices.push_back(leftBottom);
		vertices.push_back(rightTop);

		vertices.push_back(rightBottom);
		vertices.push_back(rightTop);
		vertices.push_back(leftBottom);

		char c = text[i];
		float uvX = (c%16) / 16.0f;
		float uvY = (c/16) / 16.0f;

		const float factor = 1.0f/16.0f;
		glm::vec2 uvLeftTop(uvX, 1.0f - uvY);
		glm::vec2 uvRightTop(uvX+factor, 1.0f - uvY);
		glm::vec2 uvRightBottom(uvX+factor, 1.0f - (uvY+factor));
		glm::vec2 uvLeftBottom(uvX, 1.0f - (uvY+factor));

		uvs.push_back(uvLeftTop);
		uvs.push_back(uvLeftBottom);
		uvs.push_back(uvRightTop);
		
		uvs.push_back(uvRightBottom);
		uvs.push_back(uvRightTop);
		uvs.push_back(uvLeftBottom);
	}
	
	/*
	for(unsigned int i=0; i < vertices.size();i++)
	{
		P("v.x=%f,v.y=%f,uv.x=%f,uv.y=%f\n", vertices[i].x, vertices[i].y, uvs[i].x, uvs[i].y);
	}
	*/

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvBufferId);
	glBufferData(GL_ARRAY_BUFFER, uvs.size()*sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glUseProgram(programId);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glUniform1i(textureImageId, 0);

	glEnableVertexAttribArray(positionId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glVertexAttribPointer(positionId, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(uvId);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferId);
	glVertexAttribPointer(uvId, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	


}

void DestroyText2d()
{
}
