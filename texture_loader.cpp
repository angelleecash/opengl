#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glfw.h>
#include "util.h"

#include "texture_loader.h"
#define DXT1 0x31545844
#define DXT3 0x33545844
#define DXT5 0x35545844

GLuint loadDds(const char* path)
{
	P("loading dds file:%s", path);
	unsigned char header[124];
	FILE* fp;

	fp = fopen(path, "rb");

	char fileCode[5];
	fileCode[4] = '\0';
	fread(fileCode, 1, 4, fp);

	P("file code:%s\n", fileCode);

	fread(header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&header[8];
	unsigned int width = *(unsigned int*)&header[12];
	unsigned int linearSize = *(unsigned int*)&header[16];
	unsigned int mipMapCount = *(unsigned int*)&header[24];
	unsigned int fourCc = *(unsigned int*)&header[80];

	unsigned char* buffer;
	unsigned int bufferSize;

	bufferSize = mipMapCount > 1 ? linearSize*2 : linearSize;
	buffer = (unsigned char*) malloc(bufferSize*sizeof(unsigned char));
	fread(buffer, 1, bufferSize, fp);

	fclose(fp);

	unsigned int components = (fourCc == DXT1) ? 3 : 4;
	unsigned int format;

	switch(fourCc)
	{
		case DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
	}

	GLuint textureId;
	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;


	for(unsigned int level=0; level < mipMapCount && (width || height);level++)
	{
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer+offset);
		offset += size;
		width /= 2;
		height /= 2;
	}

	free(buffer);
	return textureId;
}



GLuint LoadTga(const char* path)
{
	P("Loading tga %s\n", path);
	GLuint textureId;
	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	int result = glfwLoadTexture2D(path, GLFW_BUILD_MIPMAPS_BIT);

	//int result = glfwLoadTexture2D(path, 0);

	P("result %d\n", result);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	//glGenerateMipmap(GL_TEXTURE_2D);

	return textureId;
}



