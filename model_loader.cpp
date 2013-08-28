#include <vector>
#include <string.h>

#include <GL/glew.h>
#include <GL/glfw.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



#include "util.h"
#include "model_loader.h"
bool LoadObj(const char* path, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals)
{
	P("Loading model %s \n", path);
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	
	std::vector<glm::vec3> tmpVertices;
	std::vector<glm::vec2> tmpUvs;
	std::vector<glm::vec3> tmpNormals;

	FILE* file = fopen(path, "r");
	if(!file)
	{
		return false;
	}

	while(true)
	{
		char lineHeader[128];
		int result = fscanf(file, "%s", lineHeader);
		if(result == EOF)
		{
			break;
		}
		if(strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tmpVertices.push_back(vertex);
		}
		else if(strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y;
			tmpUvs.push_back(uv);
		}
		else if(strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tmpNormals.push_back(normal);
		}
		else if(strcmp(lineHeader, "f") == 0)
		{
			//std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],&vertexIndex[1], &uvIndex[1], &normalIndex[1],&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if(matches != 9)
			{
				P("Unable to parse %s\n", path);
				return false;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);

			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else 
		{
			char __[1024];
			fgets(__, 1000, file);
		}
	}

	for(unsigned int i=0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		glm::vec3 vertex = tmpVertices[vertexIndex-1];
		glm::vec2 uv = tmpUvs[uvIndex-1];
		glm::vec3 normal = tmpNormals[normalIndex-1];

		vertices.push_back(vertex);
		uvs.push_back(uv);
		normals.push_back(normal);
	}
	return true;
}
