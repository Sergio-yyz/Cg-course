#pragma once

#include <vector>
#include <queue>
#include <iostream>
#include <glad/glad.h>

#include <glm/glm.hpp>

#include "../base/vertex.h"
#include "../base/shader.h"
#include "../base/object3d.h"

class channel {
public:
	std::vector<Collision> box;
	channel();
	channel(GLfloat   h, GLfloat   r, GLuint _segments);
	~channel();
	void draw(const glm::mat4& projection, const glm::mat4& view);
private:
	GLuint _vao = 0;
	GLuint _vbo = 0;
	GLfloat _h;
	GLfloat _r;
	std::vector<float> channelvertices;
	GLuint _segment;

	const char* _vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"uniform mat4 projection;\n"
		"uniform mat4 view;\n"
		"void main() {\n"
		"   vec4 pos = projection * view *vec4(aPosition, 1.0f);\n"
		"   gl_Position = pos.xyzw;\n"
		"}\n";

	const char* _fsCode =
		"#version 330 core\n"
		"out vec4 fragColor;\n"
		"void main() {\n"
		"	fragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
		"}\n";

	Shader _shader = { _vsCode, _fsCode };

	void cleanup();
};