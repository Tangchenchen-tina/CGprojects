// Termm--Fall 2022

#pragma once

#include <glm/glm.hpp>
#include "HitInfo.hpp"

class Primitive {
public:
  virtual ~Primitive();
  virtual bool Hit(Ray ray, HitInfo * hitInfo);
  bool bump = false;
};

class Sphere : public Primitive {
public:
  Sphere();
  virtual ~Sphere();
  Primitive * nonh_sphere;
    virtual bool Hit(Ray ray, HitInfo * hitInfo) override;

};

class Cube : public Primitive {
public:
  Cube();
  virtual ~Cube();
  Primitive * nonh_box;
    virtual bool Hit(Ray ray, HitInfo * hitInfo) override;

};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  virtual bool Hit(Ray ray, HitInfo * hitInfo) override;

private:
  glm::vec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size);
  
  virtual ~NonhierBox();
  virtual bool Hit(Ray ray, HitInfo * hitInfo) override;


private:
  glm::vec3 m_pos;
  double m_size;
  Primitive * meshBox;
};
