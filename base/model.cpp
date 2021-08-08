#include <iostream>
#include <unordered_map>

#include <tiny_obj_loader.h>
#include "../external/self_loader.h"
#include "model.h"

Model::Model(const std::string& filepath) {
	my_loader::attrib_t attrib;
	std::vector<my_loader::shape_t> shapes;

	std::string err;

	std::string::size_type index = filepath.find_last_of("/");
	std::string mtlBaseDir = filepath.substr(0, index + 1);

	if (!my_loader::LoadObj(&attrib, &shapes,filepath.c_str())) {
		throw std::runtime_error("load " + filepath + " failure: " + err);
	}

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::unordered_map<Vertex, uint32_t> uniqueVertices;

	float x_min = INFINITY;
	float x_max = -INFINITY;
	float y_min = INFINITY;
	float y_max = -INFINITY;
	float z_min = INFINITY;
	float z_max = -INFINITY;

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};

			vertex.position.x = attrib.vertices[3 * index.vertex_index + 0];
			vertex.position.y = attrib.vertices[3 * index.vertex_index + 1];
			vertex.position.z = attrib.vertices[3 * index.vertex_index + 2];

			if (index.normal_index >= 0) {
				vertex.normal.x = attrib.normals[3 * index.normal_index + 0];
				vertex.normal.y = attrib.normals[3 * index.normal_index + 1];
				vertex.normal.z = attrib.normals[3 * index.normal_index + 2];
			}

			if (index.texcoord_index >= 0) {
				vertex.texCoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
				vertex.texCoord.y = attrib.texcoords[2 * index.texcoord_index + 1];
			}

			// check if the vertex appeared before to reduce redundant data
			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);

			if (vertex.position.x > x_max) x_max = vertex.position.x;
			if (vertex.position.x < x_min) x_min = vertex.position.x;
			if (vertex.position.y > y_max) y_max = vertex.position.y;
			if (vertex.position.y < y_min) y_min = vertex.position.y;
			if (vertex.position.z > z_max) z_max = vertex.position.z;
			if (vertex.position.z < z_min) z_min = vertex.position.z;
		}
	}

	kernel.l_f_d = glm::vec3(x_min, y_min, z_max);
	kernel.r_f_d = glm::vec3(x_max, y_min, z_max);
	kernel.r_b_d = glm::vec3(x_max, y_min, z_min);
	kernel.l_b_d = glm::vec3(x_min, y_min, z_min);
	kernel.l_f_t = glm::vec3(x_min, y_max, z_max);
	kernel.r_f_t = glm::vec3(x_max, y_max, z_max);
	kernel.r_b_t = glm::vec3(x_max, y_max, z_min);
	kernel.l_b_t = glm::vec3(x_min, y_max, z_min);

	_vertices = vertices;
	_indices = indices;

	initGLResources();
}

Model::Model(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	: _vertices(vertices), _indices(indices) {
	initGLResources();
}

Model::~Model() {
	if (_ebo != 0) {
		glDeleteBuffers(1, &_ebo);
		_ebo = 0;
	}

	if (_vbo != 0) {
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
	}

	if (_vao != 0) {
		glDeleteVertexArrays(1, &_vao);
		_vao = 0;
	}
}

void Model::draw() const {
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, (GLsizei)_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

GLuint Model::getVertexArrayObject() const {
	return _vao;
}

size_t Model::getVertexCount() const {
	return _vertices.size();
}

size_t Model::getFaceCount() const {
	return _indices.size() / 3;
}

void Model::initGLResources() {
	// create a vertex array object
	glGenVertexArrays(1, &_vao);
	// create a vertex buffer object
	glGenBuffers(1, &_vbo);
	// create a element array buffer
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint32_t), _indices.data(), GL_STATIC_DRAW);

	// specify layout, size of a vertex, data type, normalize, sizeof vertex array, offset of the attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

Collision Model::getWorldKernel() {
	Collision newKernel = Collision(
		glm::vec3(this->getModelMatrix() * glm::vec4(kernel.l_f_d, 1.0f)),
		glm::vec3(this->getModelMatrix() * glm::vec4(kernel.r_f_d, 1.0f)),
		glm::vec3(this->getModelMatrix() * glm::vec4(kernel.r_b_d, 1.0f)),
		glm::vec3(this->getModelMatrix() * glm::vec4(kernel.l_b_d, 1.0f)),
		glm::vec3(this->getModelMatrix() * glm::vec4(kernel.l_f_t, 1.0f)),
		glm::vec3(this->getModelMatrix() * glm::vec4(kernel.r_f_t, 1.0f)),
		glm::vec3(this->getModelMatrix() * glm::vec4(kernel.r_b_t, 1.0f)),
		glm::vec3(this->getModelMatrix() * glm::vec4(kernel.l_b_t, 1.0f)));
	return newKernel;
}






