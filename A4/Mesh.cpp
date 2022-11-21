// Termm--Fall 2022

#include <fstream>
#include <iostream>

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <time.h>
#include <vector>

//#include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"
const float EPSILON = 0.0001;

Mesh::Mesh(const std::string &fname) : m_vertices(), m_faces() {
  std::string code;
  double vx, vy, vz;
  size_t s1, s2, s3;

  std::ifstream ifs(fname.c_str());
  while (ifs >> code) {
    if (code == "v") {
      ifs >> vx >> vy >> vz;
      m_vertices.push_back(glm::vec3(vx, vy, vz));
    } else if (code == "f") {
      ifs >> s1 >> s2 >> s3;
      // std::cout<< s1 <<" "<<s2<<" "<<s3<<std::endl;
      m_faces.push_back(Triangle(s1 - 1, s2 - 1, s3 - 1));
    }
  }

  float min_x, min_y, min_z, max_x, max_y, max_z;
  min_x = max_x = m_vertices[0].x;
  min_y = max_y = m_vertices[0].y;
  min_z = max_z = m_vertices[0].z;

  for (int i = 0; i < m_vertices.size(); i++) {
    if (m_vertices[i].x < min_x)
      min_x = m_vertices[i].x;
    if (m_vertices[i].y < min_y)
      min_y = m_vertices[i].y;
    if (m_vertices[i].z < min_z)
      min_z = m_vertices[i].z;
    if (m_vertices[i].x > max_x)
      max_x = m_vertices[i].x;
    if (m_vertices[i].y > max_y)
      max_y = m_vertices[i].y;
    if (m_vertices[i].z > max_z)
      max_z = m_vertices[i].z;
  }

  boundcenter = glm::vec3(min_x, min_y, min_z);
  boundsize =
      (double)glm::max(max_z - min_z, glm::max(max_x - min_x, max_y - min_y));
  // std::cout << "Center Bounding Box " << glm::to_string(boundcenter)
  //           << std::endl;
  // std::cout << "Size Bounding Box " << boundsize << std::endl;

  nonh_box = new NonhierBox(
      glm::vec3{boundcenter.x, boundcenter.y, boundcenter.z}, boundsize);
}

std::ostream &operator<<(std::ostream &out, const Mesh &mesh) {
  out << "mesh {";
  /*

  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
        const MeshVertex& v = mesh.m_verts[idx];
        out << glm::to_string( v.m_position );
        if( mesh.m_have_norm ) {
          out << " / " << glm::to_string( v.m_normal );
        }
        if( mesh.m_have_uv ) {
          out << " / " << glm::to_string( v.m_uv );
        }
  }

*/
  out << "}";
  return out;
}

bool Mesh::boundBoxHit(Ray ray, HitInfo *hitInfo) {
  bool hit = false;
  if(bump){
    nonh_box->bump = true;
  }
  hit = (NonhierBox *)nonh_box->Hit(ray, hitInfo);

  // std::cout<<"hit Bounding Box "<<hit<<std::endl;

  return hit;
}

bool Mesh::Hit(Ray ray, HitInfo *hitInfo) {
  bool hit = false;
  if (boundVolume) {
    // boundVolume to box
    hit = boundBoxHit(ray, hitInfo);
  } else {
    float min_t = FLT_MAX;
    for (auto triangle : m_faces) {
      glm::vec3 v1 = m_vertices[triangle.v1];
      glm::vec3 v2 = m_vertices[triangle.v2];
      glm::vec3 v3 = m_vertices[triangle.v3];

      glm::vec3 normal_vec = cross((v1 - v2), (v3 - v2));
      float D = -1 * dot(normal_vec, v2);
      float t = (-1 * D - dot(normal_vec, ray.eye)) / dot(normal_vec, ray.dir);
      t = glm::max(EPSILON, t);

      if (t != EPSILON && t <= min_t && t <= hitInfo->t) {
        // hit plane
        // p = v2+(v1-v2)*s + (v3-v2)*t
        // 0<=s<=1
        // 0<=t<=1
        // s+t <= 1
        glm::vec3 p = ray.eye + t * ray.dir;
        float areaALL = 0.5 * length(cross((v1 - v2), (v3 - v2)));
        float areaV1 = 0.5 * length(cross(v3 - p, v2 - p));
        float areaV2 = 0.5 * length(cross(v1 - p, v3 - p));
        float areaV3 = 0.5 * length(cross(v1 - p, v2 - p));
        float a = areaV1 / areaALL;
        float b = areaV2 / areaALL;
        float c = areaV3 / areaALL;
        if (a >= 0 - EPSILON && a <= 1 + EPSILON && b >= 0 - EPSILON &&
            b <= 1 + EPSILON && c >= 0 - EPSILON && c <= 1 + EPSILON &&
            a + b + c >= 1 - EPSILON && a + b + c <= 1 + EPSILON) {
          //   std::cout<< "t VALUE: " <<t<<std::endl;
          // std::cout<< "ABC VALUE: " <<a<<" "<<b<<" "<<c<<std::endl;
          hit = true;
          min_t = t;
          hitInfo->t = t;

          glm::vec3 pixel = ray.dir - ray.eye;

          normal_vec = cross((v1 - v2), (v3 - v2));
          if (dot(ray.dir, normal_vec) > 0)
            normal_vec = -normal_vec;

          float rand1 = 0;
          float rand2 = 0;
          float rand3 = 0;

          if (bump) {

            srand(int(33 * (pixel.x * pixel.y + pixel.x + pixel.y +
                             pixel.x / pixel.y)));

            rand1 = (float)(std::rand() % 100) / 100 *
                    (float)(pow(-1, std::rand() % 2));

            srand(int(77 * (pixel.x * pixel.y)));

            rand2 = (float)(std::rand() % 100) / 100 *
                    (float)(pow(-1, std::rand() % 2));

            srand(int(100 * (pixel.x / pixel.y)));
            rand3 = (float)(std::rand() % 100) / 100;
          }

          hitInfo->hitNormal =
              normalize(normal_vec) + glm::vec3(rand1, rand2, rand3);
                        std::cout << glm::to_string(pixel) << std::endl;

          // std::cout << glm::to_string(normalize(normal_vec)) << std::endl;
          // std::cout << rand1 << " " << rand2 << std::endl;
          // std::cout << glm::to_string(hitInfo->hitNormal) << std::endl;
          // std::cout << std::endl;

          hitInfo->hitPixel = ray.eye + t * ray.dir;
        }
      }
    }
  }
  return hit;
}

Mesh::Mesh(std::vector<glm::vec3> &v, const std::vector<glm::vec3> &f,
           bool bv = true)
    : m_vertices(v) {
  for (size_t i = 0; i < f.size(); i++) {
    m_faces.push_back(Triangle((size_t)f[i].x, (size_t)f[i].y, (size_t)f[i].z));
  }
  if (!bv) {
    boundVolume = false;
  }
}
