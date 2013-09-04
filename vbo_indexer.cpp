#include "vbo_indexer.h"
#include <string.h>
#include "util.h"

typedef struct VertexPack
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;

	bool operator<(const VertexPack that) const
	{
		return memcmp((void*)this, (void*)&that, sizeof(VertexPack)) > 0;
	}
} VertexPack;


static bool findSimilarVertex(VertexPack& pack, std::map<VertexPack, unsigned short>& vertexToIndex, unsigned short& index)
{
	std::map<VertexPack, unsigned short>::iterator it = vertexToIndex.find(pack);
	if(it == vertexToIndex.end())
	{
		return false;
	}
	else
	{
		index = it->second;
		return true;
	}
}

void indexVbo(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<unsigned short>& indices, std::vector<glm::vec3>& indexedVertices, std::vector<glm::vec2>& indexedUvs, std::vector<glm::vec3>& indexedNormals)
{
	std::map<VertexPack, unsigned short> vertexToIndex;
	for(unsigned int i=0; i < vertices.size(); i++)
	{
		unsigned short index;

		VertexPack pack = {vertices[i], uvs[i], normals[i]};

		bool found = findSimilarVertex(pack, vertexToIndex, index);
		if(found)
		{
			//P("Similar vertex found for %d\n", i);
			indices.push_back(index);
		}
		else
		{
			indexedVertices.push_back(vertices[i]);
			indexedUvs.push_back(uvs[i]);
			indexedNormals.push_back(normals[i]);
			indices.push_back((unsigned short)(indexedVertices.size()-1));
			vertexToIndex[pack] = indices[indices.size()-1];
		}
	}

	P("vbo index from vertex %d to indexed vertex %d indices %d\n", vertices.size(), indexedVertices.size(), indices.size());
}

bool IsNear(float v1, float v2)
{
	return fabs(v1-v2) < 0.01f;
}

static bool FindSimilarVertex(glm::vec3& vertex, glm::vec2& uv, glm::vec3& normal, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, unsigned short& result)
{
	for(unsigned int i=0; i < vertices.size(); i++)
	{
		if(IsNear(vertex.x, vertices[i].x) &&
		   IsNear(vertex.y, vertices[i].y) &&
			IsNear(vertex.z, vertices[i].z) &&
			IsNear(uv.x, uvs[i].x) &&
			IsNear(uv.y, uvs[i].y) &&
			IsNear(normal.x, normals[i].x) &&
			IsNear(normal.y, normals[i].y) &&
			IsNear(normal.z, normals[i].z) )
		{
			result = i;
			return true;
		}
	}

	return false;
}

void indexVboTangent(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals,std::vector<glm::vec3>& tangents,std::vector<glm::vec3>& bitangents, std::vector<unsigned short>& indices, std::vector<glm::vec3>& indexedVertices, std::vector<glm::vec2>& indexedUvs, std::vector<glm::vec3>& indexedNormals, std::vector<glm::vec3>& indexedTangents,std::vector<glm::vec3>& indexedBitangents)
{
	for(unsigned int i=0; i < vertices.size(); i++)
	{
		unsigned short index;
		bool found = FindSimilarVertex(vertices[i], uvs[i], normals[i], indexedVertices, indexedUvs, indexedNormals, index);
		if(found)
		{
			indices.push_back(index);

			indexedTangents[index] += tangents[i];
			indexedBitangents[index] += bitangents[i];
		}
		else
		{
			indexedVertices.push_back(vertices[i]);
			indexedUvs.push_back(uvs[i]);
			indexedNormals.push_back(normals[i]);
			indexedTangents.push_back(tangents[i]);
			indexedBitangents.push_back(bitangents[i]);
			indices.push_back((unsigned short)(indexedVertices.size()-1));
		}
	}
}
