// Termm--Fall 2022

#include <glm/ext.hpp>

#include "A4.hpp"
#include "HitInfo.hpp"

using namespace glm;
using namespace std;

void A4_Render(
    // What to render
    SceneNode *root,

    // Image to write to, set to a given width and height
    Image &image,

    // Viewing parameters
    const glm::vec3 &eye, const glm::vec3 &view, const glm::vec3 &up,
    double fovy,

    // Lighting parameters
    const glm::vec3 &ambient, const std::list<Light *> &lights) {

  // Fill in raytracing code here...

  std::cout << "F22: Calling A4_Render(\n"
            << "\t" << *root << "\t"
            << "Image(width:" << image.width() << ", height:" << image.height()
            << ")\n"
               "\t"
            << "eye:  " << glm::to_string(eye) << std::endl
            << "\t"
            << "view: " << glm::to_string(view) << std::endl
            << "\t"
            << "up:   " << glm::to_string(up) << std::endl
            << "\t"
            << "fovy: " << fovy << std::endl
            << "\t"
            << "ambient: " << glm::to_string(ambient) << std::endl
            << "\t"
            << "lights{" << std::endl;

  for (const Light *light : lights) {
    std::cout << "\t\t" << *light << std::endl;
  }
  std::cout << "\t}" << std::endl;
  std::cout << ")" << std::endl;

  size_t h = image.height();
  size_t w = image.width();
	size_t depth = (h/2)/tan(radians(fovy*0.5));

  // normallized: normal vector, view vector, light vector, reflection vector
  vec3 normView = normalize(view);
  vec3 normEye = normalize(eye);

  for (uint y = 0; y < h; ++y) {
    for (uint x = 0; x < w; ++x) {
			vec3 pixel = vec3{x-(w/2), y-(h/2), normView.z*depth* abs(y-(h/2))};
			Ray ray = Ray{eye, pixel-eye}; // normalize?


      // Red:
      image(x, y, 0) = (double)1.0;
      // Green:
      image(x, y, 1) = (double)1.0;
      // Blue:
      image(x, y, 2) = (double)1.0;
    }
  }
}
