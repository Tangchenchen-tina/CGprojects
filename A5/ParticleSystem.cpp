#include "ParticleSystem.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"
#include <glm/glm.hpp>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <random>
#include <stack>
#include <string.h>

ParticleSystem::ParticleSystem()
    : maxParticles(1000), lastUsedParticle(0), particlesCount(0){
    };

int ParticleSystem::findUnusedParticle() {
  for (int i = lastUsedParticle; i < maxParticles; i++) {
    if (particlesContainer[i].life <= 0) {
      lastUsedParticle = i;
      return i;
    }
  }
  for (int i = 0; i < lastUsedParticle; i++) {
    if (particlesContainer[i].life <= 0) {
      lastUsedParticle = i;
      return i;
    }
  }
  return 0;
}

void ParticleSystem::createParticle(int mode,int totalnum, vec3 pos, vec3 speed, float size, float life){
  for(int i=0; i<totalnum; i++){
    int particle_idx = findUnusedParticle();
      float newlife = std::max(10, std::rand()%int(life)+(int)life/2);
     // std::cout<<newlife<<std::endl;
    particlesContainer[particle_idx].resetParticle(mode, pos, speed, size, newlife);
  }
}