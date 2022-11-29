// Termm-Fall 2022

#pragma once

#include <glm/glm.hpp>

// Material surface properties to be used as input into a local illumination model
// (e.g. the Phong Reflection Model).
struct Material {
	Material()
			: kd(glm::vec3(0.0f)),
			  ks(glm::vec3(0.0f)),
			  shininess(0.0f) { }

		Material(glm::vec3 d, glm::vec3 s, float shine)
			: kd(d),
			  ks(s),
			  shininess(shine) { }

	// Diffuse reflection coefficient
	glm::vec3 kd;

	// Specular reflection coefficient
	glm::vec3 ks;

	// Material shininess constant.  Larger positive values model surfaces that
	// are smoother or mirror-like.  Smaller positive values model rougher surfaces.
	float shininess;
};
