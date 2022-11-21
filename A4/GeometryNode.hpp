// Termm--Fall 2022

#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim, 
		Material *mat = nullptr );

	void setMaterial( Material *material );
	bool Hit(Ray ray, HitInfo *hitInfo, glm::mat4 transMat);
	void set_bump();

	Material *m_material;
	Primitive *m_primitive;
};
