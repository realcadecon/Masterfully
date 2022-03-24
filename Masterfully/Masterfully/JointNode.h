#pragma once

#include <inc/Kinect.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class JointNode {
public:

	class Limb {
	public:
		glm::vec3 norm;
		float length;
		JointNode* node;

		Limb(glm::vec3 norm, float length, JointNode* node) : norm(norm), length(length) {
			this->node = node;
		}
	};

	JointType type;
	glm::vec3 pos;
	std::vector<Limb*> limbs;

	JointNode(JointType type): type(type), pos(glm::vec3(0, 0, 0)) {}
};