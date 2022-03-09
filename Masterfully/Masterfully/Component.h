#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <vector>
#include <string>
#include <memory>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MatrixStack.h"
#include "Program.h"
#include "Shape.h"

#define PI 3.14159265

class Component {
public:
	std::vector<std::shared_ptr<Component>> children;
	std::string id;
	glm::vec3 tp; // translation of joint wrt parent joint
	glm::vec3 ang; // current join angles
	glm::vec3 tm; // translation of mesh wrt component joint
	glm::vec3 scale; // scaling factors for the mesh
	float rCount = 0;

	Component(std::string id);
	void addComponent(std::shared_ptr<Component> c);
	void draw(std::shared_ptr<MatrixStack> MV, std::shared_ptr<Program> prog, std::shared_ptr<Shape> shape, std::shared_ptr<Shape> sphere, std::shared_ptr<Component> selected, double selectedMod, std::vector<std::shared_ptr<Component>> &dfsOrder, std::vector<int> &spinners);
};

#endif // !_COMPONENT_H_
