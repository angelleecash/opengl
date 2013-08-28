#ifndef __MODEL_LOADER__
#define __MODEL_LOADER__

#include <vector>


bool LoadObj(const char* path, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals);

#endif

