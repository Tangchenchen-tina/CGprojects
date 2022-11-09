// Termm-Fall 2022

#pragma once

#include "SceneNode.hpp"

class JointNode : public SceneNode {
public:
	JointNode(const std::string & name);
	virtual ~JointNode();

	void set_joint_x(double min, double init, double max);
	void set_joint_y(double min, double init, double max);
	void set_joint_z(double min, double init, double max);
	bool updateAngle(float angle, int idx);
	void reset();


	struct JointRange {
		double min, init, max;
	};


	JointRange m_joint_x, m_joint_y, m_joint_z;
	float angle_x;
	float angle_y;
	float angle_z;
};
