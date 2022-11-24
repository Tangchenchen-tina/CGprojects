#include "GeometryNode.hpp"
#include "JointNode.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"
#include "Particle.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <iostream>
using namespace glm;

Particle::Particle()
    : pos(0), speed(0), col(1, 0, 0, 1), life(-1), size(0.5){};

void Particle::resetParticle(int mode, vec3 pos, vec3 speed, float size, float life){
  this->pos = pos;
  this->initpos = pos;
  float angle = std::rand()*PI / RAND_MAX;
  float angle2 = std::rand()* 2 *PI/RAND_MAX;
  //std::cout<<angle<<std::endl;
  this->speed = vec3(sin(angle)*cos(angle)/10, sin(angle)*sin(angle2)/10, (cos(angle)/10))+speed;
  this->speed.z = std::max((float)0.04, this->speed.z);
  //this->speed = speed;
  this->size = size;
  this->life = life;
  this->initlife = life;
  this->mode = mode;
}

void Particle::reActive(vec3 move){
  this->pos = this->initpos+move;
  float interval = std::max(13, std::rand()%int(initlife)+(int)initlife/2);
  this->life = interval;
}