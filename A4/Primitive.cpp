// Termm--Fall 2022

#include "Primitive.hpp"

using namespace glm;
using namespace std;

Primitive::~Primitive()
{
}

Sphere::Sphere(){
  nonh_sphere = new NonhierSphere(vec3(0,0,0), 1);
}

Sphere::~Sphere()
{
}


Cube::Cube(){
  nonh_box = new NonhierBox(vec3(0,0,0),1);
}

Cube::~Cube()
{
  
}

NonhierSphere::~NonhierSphere()
{
}

NonhierBox::~NonhierBox()
{
}
