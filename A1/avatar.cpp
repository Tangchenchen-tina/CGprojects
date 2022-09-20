#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

#include "avatar.hpp"
using namespace std;
static const size_t DIM = 16;

Avatar::Avatar() {}

Avatar::Avatar(const Avatar &a) {
  h_count = a.h_count;
  v_count = a.v_count;
  center_x = a.center_x;
  center_y = a.center_y;
  center_z = a.center_z;
  radius = a.radius;
  vertices = a.vertices;
  indices = a.indices;
}

Avatar &Avatar::operator=(const Avatar &a) {
  h_count = a.h_count;
  v_count = a.v_count;
  center_x = a.center_x;
  center_y = a.center_y;
  center_z = a.center_z;
  radius = a.radius;
  vertices = a.vertices;
  indices = a.indices;
}

void Avatar::updateInfo(Avatar &a) {
  h_count = a.h_count;
  v_count = a.v_count;
  center_x = a.center_x;
  center_y = a.center_y;
  center_z = a.center_z;
  radius = a.radius;
  vertices = a.vertices;
  indices = a.indices;
}

Avatar::Avatar(float x_init, float y_init, float z_init, float r, int h_count,
               int v_count)
    : h_count(h_count), v_count(v_count), center_x(x_init), center_y(y_init),
      center_z(z_init), radius(r) {
  const float PI = acos(-1);
  float vangle = 2 * PI / v_count;
  float hangle = PI / h_count;

  float init_hangle, curr_hangle, init_vangle, curr_vangle;
  float x, y, z, xz;
  init_vangle = 0;
  // Get vectices
  for (int v = 0; v < v_count + 1; ++v) {
    curr_vangle = init_vangle + v * vangle;
    init_hangle = PI / 2;
    for (int h = 0; h < h_count + 1; ++h) {
      curr_hangle = init_hangle - h * hangle;
      y = r * sinf(curr_hangle) + y_init;
      xz = r * cosf(curr_hangle);
      z = xz * sinf(curr_vangle) + z_init;
      x = xz * cosf(curr_vangle) + x_init;

      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);
    }
  }

  // Get index of triangles
  //  *----*
  //  |   -|
  //  | -  |
  //  *----*
  for (int i = 0; i < (v_count + 1) * (h_count + 1); ++i) {
    int ul = i;
    int bl = i + 1;
    int ur = i + h_count + 1;
    int br = i + h_count + 2;
    if (br < (v_count + 1) * (h_count + 1)) {
      // left rectangle
      indices.push_back(ul);
      indices.push_back(bl);
      indices.push_back(ur);
      // right rectangle
      indices.push_back(bl);
      indices.push_back(ur);
      indices.push_back(br);
    }
  }
}

void Avatar::moveAvatar(float x_amount, float y_amount, bool init = false) {
  float curr_x = x_amount;
  float curr_y = y_amount;
  for (int i = 0; i < 3 * (v_count + 1) * (h_count + 1); ++i) {
    if (i % 3 == 0) {
      if (center_x - radius + x_amount < 0)
        curr_x = radius - center_x;
      if (center_x + radius + x_amount > DIM)
        curr_x = DIM - radius - center_x;

      if (init)
        vertices[i] += (x_amount-center_x);
      else
        vertices[i] += curr_x;

    } else if (i % 3 == 2) {
      if (center_z - radius + y_amount < 0)
        curr_y = radius - center_z;
      if (center_z + radius + y_amount > DIM)
        curr_y = DIM - radius - center_z;

      if (init)
        vertices[i] += (y_amount-center_z);
      else
        vertices[i] += curr_y;
    }
  }
  if(init){
    center_x = x_amount;
    center_z = y_amount;
  }else{
  center_x += curr_x;
  center_z += curr_y;
  }
}

vector<GLfloat> & Avatar::getVertices() { return vertices; }

vector<unsigned int> &Avatar::getIndices() { return indices; }

Avatar::~Avatar() {}