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
	P("before index vertex count %d \n", vertices.size());
	for(unsigned int i=0; i < vertices.size(); i++)
	{
		//P("processing %d \n", i);
		unsigned short index;
		//bool found = findSimilarVertex(vertices[i], uvs[i], normals[i], indexedVertices, indexedUvs, indexedNormals, index);

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

	P("after index vertex count %d \n", indexedVertices.size());
}
