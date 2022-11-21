// Termm-Fall 2022

#include "JointNode.hpp"

//---------------------------------------------------------------------------------------
JointNode::JointNode(const std::string& name)
	: SceneNode(name)
{
	m_nodeType = NodeType::JointNode;
	angle_x = 0;
	angle_y = 0;
	angle_z = 0;
}

//---------------------------------------------------------------------------------------
JointNode::~JointNode() {

}
 //---------------------------------------------------------------------------------------
void JointNode::set_joint_x(double min, double init, double max) {
	m_joint_x.min = min;
	m_joint_x.init = init;
	m_joint_x.max = max;
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_y(double min, double init, double max) {
	m_joint_y.min = min;
	m_joint_y.init = init;
	m_joint_y.max = max;
}

void JointNode::set_joint_z(double min, double init, double max) {
	m_joint_z.min = min;
	m_joint_z.init = init;
	m_joint_z.max = max;
}

void JointNode::reset(){
	angle_x = 0;
	angle_y = 0;
	angle_z = 0;
}


bool JointNode::updateAngle(float angle, int idx){
	if(idx == 0){
		// updateX
		float tmp = angle_x + angle;
		if(tmp >= m_joint_x.min && tmp <= m_joint_x.max){
			angle_x = tmp;
			return true;
		}else if(tmp < m_joint_x.min){
			angle_x = m_joint_x.min;
			return false;
		}else if(tmp > m_joint_x.max){
			angle_x = m_joint_x.max;
			return false;
		}
	}else if(idx == 1){
		// updateY
		float tmp = angle_y + angle;
		if(tmp >= m_joint_y.min && tmp <= m_joint_y.max){
			angle_y = tmp;
			return true;
		}else if(tmp < m_joint_y.min){
			angle_y = m_joint_y.min;
			return false;
		}else if(tmp > m_joint_y.max){
			angle_y = m_joint_y.max;
			return false;
		}
	}else if(idx == 2){
		// updateY
		float tmp = angle_z + angle;
		if(tmp >= m_joint_z.min && tmp <= m_joint_z.max){
			angle_z = tmp;
			return true;
		}else if(tmp < m_joint_z.min){
			angle_z = m_joint_z.min;
			return false;
		}else if(tmp > m_joint_z.max){
			angle_z = m_joint_z.max;
			return false;
		}
	}
}