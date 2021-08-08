#pragma once

#include <vector>
#include <queue>
#include <iostream>
#include <glad/glad.h>

#include <glm/glm.hpp>

#include "../base/vertex.h"
#include "../base/shader.h"
#include "../base/object3d.h"

class sphere {
public:
	sphere() = default;
	~sphere() = default;
	void draw();
private:
	GLuint _vao = 0;
	GLuint _vbo = 0;
	int count = 0;
};