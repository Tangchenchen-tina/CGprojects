#pragma once

#include "Particle.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"
#include <glm/glm.hpp>

class ParticleSystem{
  public:
    int maxParticles;
    Particle particlesContainer[1000];
    int lastUsedParticle;
    int particlesCount;

    ParticleSystem();
    int findUnusedParticle();
    void createParticle(int mode, int totalnum, vec3 pos, vec3 speed, float size, float life);
};