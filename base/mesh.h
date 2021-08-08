#pragma once

#include <vector>

#include <glad/glad.h>

#include "vertex.h"
#include "texture.h"

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture> textures;

	GLuint vao = 0;

private:
	GLuint _vbo = 0;
	GLuint _ebo = 0;
};