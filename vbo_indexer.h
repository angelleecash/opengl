#ifndef __VBO_INDEXER__
#define __VBO_INDEXER__

#include <vector>
#include <map>

#include "glm/glm.hpp"

void indexVbo(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<unsigned short>& indices, std::vector<glm::vec3>& indexedVertices, std::vector<glm::vec2>& indexedUvs, std::vector<glm::vec3>& indexedNormals);

void indexVboTangent(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals,std::vector<glm::vec3>& tangents,std::vector<glm::vec3>& bitangents, std::vector<unsigned short>& indices, std::vector<glm::vec3>& indexedVertices, std::vector<glm::vec2>& indexedUvs, std::vector<glm::vec3>& indexedNormals, std::vector<glm::vec3>& indexedTangents,std::vector<glm::vec3>& indexedBitangents);

#endif

