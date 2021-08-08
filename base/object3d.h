#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

class Object3D {
public:
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

	glm::vec3 getFront() const;

	glm::vec3 getUp() const;

	glm::vec3 getRight() const;

	virtual glm::mat4 getModelMatrix() const;
};

class Collision {
public:
	glm::vec3 l_f_d;
	glm::vec3 r_f_d;
	glm::vec3 r_b_d;
	glm::vec3 l_b_d;
	glm::vec3 l_f_t;
	glm::vec3 r_f_t;
	glm::vec3 r_b_t;
	glm::vec3 l_b_t;
	Collision() = default;
	Collision(glm::vec3 x1, glm::vec3 x2, glm::vec3 x3, glm::vec3 x4, glm::vec3 x5, glm::vec3 x6, glm::vec3 x7, glm::vec3 x8):
		l_f_d(x1), r_f_d(x2), r_b_d(x3), l_b_d(x4), l_f_t(x5), r_f_t(x6), r_b_t(x7), l_b_t(x8){}
};