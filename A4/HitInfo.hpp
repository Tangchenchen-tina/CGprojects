#ifndef HitInfo_hpp
#define HitInfo_hpp

#include "Material.hpp"
#include <glm/glm.hpp>

struct HitInfo{
  float t;
  glm::vec3 hitPixel;
  glm::vec3 hitNormal;
  Material *material;
};


struct Ray{
	glm::vec3 eye;
	glm::vec3 dir;
};


#endif