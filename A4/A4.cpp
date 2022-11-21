// Termm--Fall 2022

#include <glm/ext.hpp>

#include "A4.hpp"
#include "HitInfo.hpp"

using namespace glm;
using namespace std;

const float EPSILON = 0.001;

bool CheckHit(SceneNode *root, Ray ray, HitInfo *info, mat4 transMat) {
  bool check = false;
  if (root->m_nodeType == NodeType::GeometryNode) {
    return root->Hit(ray, info, transMat);
  } else {
    for (SceneNode *node : root->children) {
      mat4 trans = transMat * node->get_transform();
      bool result = CheckHit(node, ray, info, trans);
      if (!check) {
        check = result;
      }
    }
  }
  return check;
}

vec3 RenderLight(Ray ray, HitInfo *info, const std::list<Light *> &lights,
                 SceneNode *root, const vec3 &ambient, int reftimes) {
  // TODO: ambient
  PhongMaterial *phm = (PhongMaterial *)(info->material);
  vec3 color = phm->getKD() * ambient;
  for (Light *lgt : lights) {
    Ray lightRay = Ray{info->hitPixel, lgt->position - info->hitPixel};
    HitInfo lightInfo;
    bool hit = CheckHit(root, lightRay, &lightInfo,root->get_transform());
    if (!hit) {
      vec3 n = normalize(info->hitNormal);
      vec3 l = normalize(lightRay.dir);
      vec3 r = normalize(-l + 2 * dot(l, n) * n);
      vec3 v = normalize(ray.eye - info->hitPixel);
      double rr = length(lightRay.dir);
      float attenuation =
          1.0f / (lgt->falloff[2] * rr * rr + lgt->falloff[1] * rr +
                  (float)lgt->falloff[0]);
      color += phm->getKD() * glm::max(0.0f, (float)dot(l, n)) * lgt->colour *
               attenuation;
      color += phm->getKS() *
               pow(glm::max((float)dot(r, v), 0.0f), phm->getShine()) *
               attenuation * lgt->colour;
    }
  }

  if (reftimes > 0) {
    vec3 l = -ray.dir;
    vec3 reflectLight =
        -l + 2 * dot(l, info->hitNormal) * info->hitNormal;
    Ray refRay = Ray{info->hitPixel, reflectLight};
    HitInfo refInfo;
          refInfo.t = FLT_MAX;
    if (CheckHit(root, refRay, &refInfo,root->get_transform())) {
      cout << "SUCCESS hit object for " << reftimes << " time" << endl;

        // cout <<"Reflect Point = " << info->hitPixel.x << " " << info->hitPixel.y << " "
        //      << info->hitPixel.z << endl;
        //         cout <<"Reflect Light = " << reflectLight.x << " " << reflectLight.y << " "
        //      << reflectLight.z << endl;
        // cout <<"Arrive Point = " << refInfo.hitPixel.x << " " << refInfo.hitPixel.y << " "
        //      << refInfo.hitPixel.z << endl;

      float refcoefficient = 0;
      if(root->mirror){
        refcoefficient = 0.4;
      }
      color = glm::mix(
          color,
          RenderLight(refRay, &refInfo, lights, root, ambient, reftimes - 1),
          refcoefficient);
    }
  }
  return color;
}

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

  vec3 startColor = vec3(0.047, 0.482, 0.702);
  vec3 endColor = vec3(0.949, 0.729, 0.91);

  float depth = ((float)h / 2) / tan(radians(fovy * 0.5));

  // normallized: normal vector, view vector, light vector, reflection vector
  vec3 normView = normalize(view);
  vec3 normEye = normalize(eye);

  for (uint y = 0; y < h; ++y) {
    for (uint x = 0; x < w; ++x) {
      const float xval = (float)(((float)w / 2) - (float)x) * -1;
      const float yval = (float)(-(float)y + ((float)h / 2));
      vec3 pixel = vec3(xval, yval, normView.z * depth);

      Ray ray = Ray{eye, pixel}; // normalize?

      HitInfo info;
      info.t = FLT_MAX;
      vec3 color = vec3(0.7, 0.7, 0.7); // += material->diffuse() * ambient;
        // cout << "HIT here: "  << endl;

      if (CheckHit(root, ray, &info, root->get_transform())) {
        // cout << "HIT SUCCESS: " << info.t << endl;
        // cout << info.hitPixel.x << " " << info.hitPixel.y << " "
        //      << info.hitPixel.z << endl;

        color = RenderLight(ray, &info, lights, root, ambient, 1);
        // PhongMaterial *phm = (PhongMaterial *)(info.material);
        // color = phm->getKD();
        // cout << endl;
      }else{
        // Paint background
        float fraction = sqrt(x*x+y*y)/sqrt(w*w+y*y);
        vec3 curr_color = startColor + fraction*(endColor-startColor);
        color = curr_color;
      }

      // Red:g
      image(x, y, 0) = (double)color.x;
      // Green:
      image(x, y, 1) = (double)color.y;
      // Blue:
      image(x, y, 2) = (double)color.z;
    }
    std::cout<<"Percent Progress: " << 100*(float)y/h << "%" << std::endl;
  }
}
