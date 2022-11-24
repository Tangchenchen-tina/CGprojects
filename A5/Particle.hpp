#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"
#include <glm/glm.hpp>
using namespace glm;

class Particle {
public:
  Particle();
  void resetParticle(int mode, vec3 pos, vec3 speed, float size, float life);
  void reActive(vec3 move);
  glm::vec3 pos, initpos, speed;
  glm::vec4 col;
  float size;
  float life; // Remaining life of the particle. if < 0 : dead and unused.
  float initlife;
  int mode;
};