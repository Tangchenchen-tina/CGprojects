// Termm--Fall 2022

#include "GeometryNode.hpp"

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(const std::string &name, Primitive *prim,
                           Material *mat)
    : SceneNode(name), m_material(mat), m_primitive(prim) {
  m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial(Material *mat) {
  // Obviously, there's a potential memory leak here.  A good solution
  // would be to use some kind of reference counting, as in the
  // C++ shared_ptr.  But I'm going to punt on that problem here.
  // Why?  Two reasons:
  // (a) In practice we expect the scene to be constructed exactly
  //     once.  There's no reason to believe that materials will be
  //     repeatedly overwritten in a GeometryNode.
  // (b) A ray tracer is a program in which you compute once, and
  //     throw away all your data.  A memory leak won't build up and
  //     crash the program.

  m_material = mat;
}

bool GeometryNode::Hit(Ray ray, HitInfo *hitInfo, glm::mat4 transMat) {
  ray.eye = glm::vec3(glm::inverse(transMat) * glm::vec4(ray.eye, 1.0));
  ray.dir = glm::vec3(glm::inverse(transMat) * glm::vec4(ray.dir, 1.0));

  if (m_primitive->Hit(ray, hitInfo)) {
    hitInfo->material = m_material;
    hitInfo->hitNormal =
        glm::vec3(glm::mat3(transpose(glm::inverse(transMat))) * hitInfo->hitNormal);
    hitInfo->hitPixel = glm::vec3(transMat * glm::vec4(hitInfo->hitPixel, 1.0));
    return true;
  } else {
    return false;
  }
}
