#include <vector>
#include <string.h>

#include <GL/glew.h>
#include <GL/glfw.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util.h"
#include "tangent.h"


void ComputeTangents(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& tangents,std::vector<glm::vec3>& bitangents)
{
	for(unsigned int i=0; i < vertices.size(); i+=3)
	{
		glm::vec3& v0 = vertices[i];
		glm::vec3& v1 = vertices[i+1];
		glm::vec3& v2 = vertices[i+2];

		glm::vec2& uv0 = uvs[i];
		glm::vec2& uv1 = uvs[i+1];
		glm::vec2& uv2 = uvs[i+2];

		glm::vec3 deltaPos1 = v1-v0;
		glm::vec3 deltaPos2 = v2-v0;

		glm::vec2 deltaUv1 = uv1-uv0;
		glm::vec2 deltaUv2 = uv2-uv0;

		float r = 1.0f / (deltaUv1.x*deltaUv2.y - deltaUv1.y*deltaUv2.x);

		glm::vec3 tangent = (deltaPos1*deltaUv2.y - deltaPos2*deltaUv1.y) * r;
		glm::vec3 bitangent = (deltaPos2*deltaUv1.x - deltaPos1*deltaUv1.y) * r;

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);
		
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}
	for(unsigned int i=0; i < vertices.size(); i+=3)
	{
		glm::vec3& n = normals[i];
		glm::vec3& t = tangents[i];
		glm::vec3& b = bitangents[i];

		t = glm::normalize(t - n*glm::dot(n,t));
		if(glm::dot(glm::cross(n,t), b) < 0.0f)
		{
			t = t*-1.0f;
		}
	}

}
