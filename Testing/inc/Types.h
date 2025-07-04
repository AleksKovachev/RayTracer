#ifndef TYPES_H
#define TYPES_H

#include <unordered_map>

class Mesh;
class Triangle;

typedef std::unordered_map<const Mesh*, std::vector<Triangle>> MeshInfo;



#endif // TYPES_H