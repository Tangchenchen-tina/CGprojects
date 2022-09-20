#pragma once

#include <vector>
#include "cs488-framework/OpenGLImport.hpp"
using namespace std;

class Avatar
{
public:
  Avatar();
	Avatar( float x, float y, float z, float r, int h_count, int v_count);
	~Avatar();
  Avatar(const Avatar &a);
  Avatar & operator=(const Avatar &a);

  void moveAvatar(float x_amount, float y_amount, bool init);
  vector<GLfloat> & getVertices();
  vector<unsigned int> &getIndices();
  void updateInfo(Avatar &a);
  float get_x(){return center_x;};
  float get_y(){return center_y;};
  float get_z(){return center_z;};
  float get_r(){return radius;};

private:
  int h_count; // split sphere horizontally
  int v_count; // split sphere vertically
  float center_x;
  float center_y;
  float center_z;
  float radius;
	vector<GLfloat> vertices;
	vector<unsigned int> indices;
};
