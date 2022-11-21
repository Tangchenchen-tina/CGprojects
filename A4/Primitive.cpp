// Termm--Fall 2022

#include "Primitive.hpp"
#include "HitInfo.hpp"
#include "Mesh.hpp"
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iosfwd>
#include <iostream>
#include <vector>

using namespace glm;

const float EPSILON = 0.00001;
const float TOLO = 0.0001;

Primitive::~Primitive() {}

bool Primitive::Hit(Ray ray, HitInfo *hitInfo) {}

Sphere::Sphere() { nonh_sphere = new NonhierSphere(vec3(0, 0, 0), 1); }

bool Sphere::Hit(Ray ray, HitInfo *hitInfo) {
  return nonh_sphere->Hit(ray, hitInfo);
}

Sphere::~Sphere() {}

Cube::Cube() { nonh_box = new NonhierBox(vec3(0, 0, 0), 1); }

bool Cube::Hit(Ray ray, HitInfo *hitInfo) {
  return nonh_box->Hit(ray, hitInfo);
}

Cube::~Cube() {}

bool NonhierSphere::Hit(Ray ray, HitInfo *hitInfo) {
  // find t
  //(e+t(p-e)-o)^2 = r^2
  vec3 origin = ray.eye - m_pos;
  float a = dot(ray.dir, ray.dir);
  float b = 2.0f * dot(origin, ray.dir);
  float c = dot(origin, origin) - m_radius * m_radius;

  // solve quadradic equation
  float b_4ac = b * b - 4 * a * c;
  if (b_4ac <= 0) {
    return false;
  } else {
    float t1 = max(0.0f, (-1 * b - sqrt(b_4ac)) / (2 * a));
    float t2 = max(0.0f, (-1 * b + sqrt(b_4ac)) / (2 * a));

    float t = min(t1, t2);
    if (t > EPSILON && t <= hitInfo->t) {
      vec3 hitPixel = ray.eye + t * ray.dir;
      hitInfo->t = t;
      hitInfo->hitPixel = hitPixel;
      float rand1 = 0;
      float rand2 = 0;
      float rand3 = 0;

      if (bump) {
        glm::vec3 pixel = ray.dir - ray.eye;

        srand(int(33 *
                  (pixel.x * pixel.y + pixel.x + pixel.y + pixel.x / pixel.y)));

        rand1 = (float)(std::rand() % 100) / 100 *
                (float)(pow(-1, std::rand() % 2));

        srand(int(77 * (pixel.x * pixel.y)));

        rand2 = (float)(std::rand() % 100) / 100 *
                (float)(pow(-1, std::rand() % 2));

        srand(int(100 * (pixel.x / pixel.y)));
        rand3 = (float)(std::rand() % 100) / 100;
      }
      hitInfo->hitNormal =
          normalize(hitPixel - m_pos) + vec3(rand1, rand2, rand3);

      return true;
    }
  }
  return false;
}

NonhierSphere::~NonhierSphere() {}

NonhierBox::NonhierBox(const glm::vec3 &pos, double size)
    : m_pos(pos), m_size(size) {
  std::vector<glm::vec3> vertices;
  vertices.push_back(m_pos + glm::vec3(0.0, 0.0, 0.0));
  vertices.push_back(m_pos + glm::vec3(m_size, 0.0, 0.0));
  vertices.push_back(m_pos + glm::vec3(m_size, 0.0, m_size));
  vertices.push_back(m_pos + glm::vec3(0.0, 0.0, m_size));
  vertices.push_back(m_pos + glm::vec3(0.0, m_size, 0.0));
  vertices.push_back(m_pos + glm::vec3(m_size, m_size, 0.0));
  vertices.push_back(m_pos + glm::vec3(m_size, m_size, m_size));
  vertices.push_back(m_pos + glm::vec3(0.0, m_size, m_size));

  std::vector<glm::vec3> triangle = {
      glm::vec3(0, 1, 2), glm::vec3(0, 2, 3), glm::vec3(0, 7, 4),
      glm::vec3(0, 3, 7), glm::vec3(0, 4, 5), glm::vec3(0, 5, 1),
      glm::vec3(6, 2, 1), glm::vec3(6, 1, 5), glm::vec3(6, 5, 4),
      glm::vec3(6, 4, 7), glm::vec3(6, 7, 3), glm::vec3(6, 3, 2)};

  meshBox = new Mesh(vertices, triangle, false);
}

bool NonhierBox::Hit(Ray ray, HitInfo *hitInfo) {
  // // check 6 faces
  // // back and front
  // int idx = 0;
  // float t1 = max((float)EPSILON, (m_pos.z - ray.eye.z) / ray.dir.z);
  // if (t1 == EPSILON)
  //   t1 = FLT_MAX;
  // float t2 =
  //     max((float)EPSILON, (m_pos.z + (float)m_size - ray.eye.z) / ray.dir.z);
  // if (t2 == EPSILON)
  //   t2 = FLT_MAX;
  // // down and up
  // float t3 = max((float)EPSILON, (m_pos.y - ray.eye.y) / ray.dir.y);
  // if (t3 == EPSILON)
  //   t3 = FLT_MAX;
  // float t4 =
  //     max((float)EPSILON, (m_pos.y + (float)m_size - ray.eye.y) / ray.dir.y);
  // if (t4 == EPSILON)
  //   t4 = FLT_MAX;
  // // left and right
  // float t5 = max((float)EPSILON, (m_pos.x - ray.eye.x) / ray.dir.x);
  // if (t5 == EPSILON)
  //   t5 = FLT_MAX;
  // float t6 =
  //     max((float)EPSILON, (m_pos.x + (float)m_size - ray.eye.x) / ray.dir.x);
  // if (t6 == EPSILON)
  //   t6 = FLT_MAX;

  // vec3 hitPixel1 = ray.eye + t1 * ray.dir;
  // vec3 hitPixel2 = ray.eye + t2 * ray.dir;
  // vec3 hitPixel3 = ray.eye + t3 * ray.dir;
  // vec3 hitPixel4 = ray.eye + t4 * ray.dir;
  // vec3 hitPixel5 = ray.eye + t5 * ray.dir;
  // vec3 hitPixel6 = ray.eye + t6 * ray.dir;

  // float t = (float)min(min(min(min(min(t1, t2), t3), t4), t5), t6);

  // std::cout << t << std::endl;
  // std::cout << t1 << " " << t2 << " " << t3 << " " << t4 << " " << t5 << " "
  //           << t6 << std::endl;

  // // if (t == EPSILON || t >= hitInfo->t) {
  // //   return false;
  // // }

  // if (t == FLT_MAX || t > hitInfo->t) {
  //   return false;
  // }

  // vec3 center = m_pos + vec3(m_size / 2, m_size / 2, m_size / 2);
  // vec3 backcenter = m_pos + vec3(m_size / 2, m_size / 2, 0);
  // vec3 downcenter = m_pos + vec3(m_size / 2, 0, m_size / 2);
  // vec3 leftcenter = m_pos + vec3(0, m_size / 2, m_size / 2);

  // if (t <= t1 + TOLO && t >= t1 - TOLO) {
  //   if (hitPixel1.x >= m_pos.x && hitPixel1.x <= m_pos.x + m_size &&
  //       hitPixel1.y >= m_pos.y && hitPixel1.y <= m_pos.y + m_size) {
  //     hitInfo->hitNormal = backcenter - center;
  //     hitInfo->t = t;
  //     hitInfo->hitPixel = ray.eye + t * ray.dir;
  //     return true;
  //   }
  // } else if (t <= t2 + TOLO && t >= t2 - TOLO) {
  //   if (hitPixel2.x >= m_pos.x && hitPixel2.x <= m_pos.x + m_size &&
  //       hitPixel2.y >= m_pos.y && hitPixel2.y <= m_pos.y + m_size) {
  //     hitInfo->hitNormal = -backcenter + center;
  //     hitInfo->t = t;
  //     hitInfo->hitPixel = ray.eye + t * ray.dir;
  //     return true;
  //   }
  // } else if (t <= t3 + TOLO && t >= t3 - TOLO) {
  //   std::cout << glm::to_string(hitPixel3) << std::endl;
  //   std::cout << glm::to_string(m_pos) << std::endl;
  //       std::cout << "ray "<< glm::to_string(ray.dir) << std::endl;

  //   if (hitPixel3.x >= m_pos.x && hitPixel3.x <= m_pos.x + m_size &&
  //       hitPixel3.z >= m_pos.z && hitPixel3.z <= m_pos.z + m_size) {
  //     hitInfo->hitNormal = downcenter - center;
  //     hitInfo->t = t;
  //     hitInfo->hitPixel = ray.eye + t * ray.dir;
  //     return true;
  //   }
  // } else if (t <= t4 + TOLO && t >= t4 - TOLO) {
  //   if (hitPixel4.x >= m_pos.x && hitPixel4.x <= m_pos.x + m_size &&
  //       hitPixel4.z >= m_pos.z && hitPixel4.z <= m_pos.z + m_size) {
  //     hitInfo->hitNormal = -downcenter + center;
  //     hitInfo->t = t;
  //     hitInfo->hitPixel = ray.eye + t * ray.dir;
  //     return true;
  //   }
  // } else if (t <= t5 + TOLO && t >= t5 - TOLO) {
  //   if (hitPixel5.y >= m_pos.y && hitPixel5.y <= m_pos.y + m_size &&
  //       hitPixel5.z >= m_pos.z && hitPixel5.z <= m_pos.z + m_size) {
  //     hitInfo->hitNormal = leftcenter - center;
  //     hitInfo->t = t;
  //     hitInfo->hitPixel = ray.eye + t * ray.dir;
  //     return true;
  //   }
  // } else if (t <= t6 + TOLO && t >= t6 - TOLO) {
  //   if (hitPixel6.y >= m_pos.y && hitPixel6.y <= m_pos.y + m_size &&
  //       hitPixel6.z >= m_pos.z && hitPixel6.z <= m_pos.z + m_size) {
  //     hitInfo->hitNormal = -leftcenter + center;
  //     hitInfo->t = t;
  //     hitInfo->hitPixel = ray.eye + t * ray.dir;
  //     return true;
  //   }
  // }
  // return false;
  meshBox->bump = bump;
  return meshBox->Hit(ray, hitInfo);
}

NonhierBox::~NonhierBox() { delete meshBox; }
