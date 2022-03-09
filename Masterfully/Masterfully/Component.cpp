#include "Component.h"
#include <iostream>

using namespace std;

Component::Component(string id) : id(id) 
{
	tp = glm::vec3(0, 0, 0);
	ang = glm::vec3(0, 0, 0);
	tm = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}

void Component::addComponent(shared_ptr<Component> c)
{
	children.push_back(c);
}

void Component::draw(shared_ptr<MatrixStack> MV, shared_ptr<Program> prog, shared_ptr<Shape> shape, shared_ptr<Shape> sphere, shared_ptr<Component> selected, double selectedMod, vector<shared_ptr<Component>> &dfsOrder, vector<int> &spinners)
{
	MV->pushMatrix();
		MV->translate(tp);
		MV->rotate(ang[0], glm::vec3(1, 0, 0));
		MV->rotate(ang[1], glm::vec3(0, 1, 0));
		MV->rotate(ang[2], glm::vec3(0, 0, 1));
		bool spin = false;
		for (auto x : spinners) {
			if (this->id == dfsOrder[x]->id) {
				spin = true;
				break;
			}
		}
		if (spin) {
			MV->pushMatrix();
			MV->rotate(rCount * PI, glm::vec3(0, 1, 0));
			rCount += 0.01;
			if (rCount == 2) {
				rCount = 0;
			}
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
			sphere->draw(prog);
			MV->popMatrix();
		}
		else {
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
			sphere->draw(prog);
		}
		MV->pushMatrix();
			MV->translate(tm);
			if (this->id == selected->id) {
				MV->scale(glm::vec3(scale[0]*selectedMod, scale[1] * selectedMod, scale[2] * selectedMod));
			}
			else {
				MV->scale(scale);
			}

			if (spin) {
				MV->pushMatrix();
				MV->rotate(rCount * PI, glm::vec3(0, 1, 0));
				rCount += 0.01;
				if (rCount == 2) {
					rCount = 0;
				}
				glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
				shape->draw(prog);
				MV->popMatrix();
			}
			else {
				glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
				shape->draw(prog);
			}
		MV->popMatrix();
		for (auto &x : children) {
			x->draw(MV, prog, shape, sphere, selected, selectedMod, dfsOrder, spinners);
		}
	MV->popMatrix();
}