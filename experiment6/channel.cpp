#include"channel.h"

const GLfloat PI = 3.141593f;
GLfloat checkmid[] = {
	-210.272f, 110.339f, 136.77f,
	-378.917f, 371.533f, 201.373f,
	-149.387f, 193.246f, 312.466f,
	161.782f,  337.101f, 294.342f,
	93.352f,   -59.467f, 345.972f,
	389.134f, -192.771f, 354.209f,
	15.901f,  -183.755f, 73.371f,
	-210.638f,-399.19f,  114.857f,
	-77.433f, -85.107f,  190.256f
};

channel::channel(GLfloat h, GLfloat r, GLuint segment) {
	_h = h;
	_r = r;
	float posx, posy, posz;
	_segment = segment;
	float midposx, midposy, midposz, midyaw;

	Collision tempc;

	for (int i = 0; i < 9; i++) {
		for (int j = -1; j < 2; j += 2) {
			midyaw = checkmid[3 * i + 2];
			midposx = checkmid[3 * i] + j * 25.0f * cos(glm::radians(midyaw));
			midposy = 0.0f;
			midposz = checkmid[3 * i + 1] - j * 25.0f * sin(glm::radians(midyaw));
			for (int k = 0; k <= segment; k++) {
				float theta = 2 * PI / (float)segment * (float)k;
				posx = std::cos(theta) * _r + midposx;
				posy = midposy;
				posz = std::sin(theta) * _r + midposz;
				channelvertices.push_back(posx);
				channelvertices.push_back(posy);
				channelvertices.push_back(posz);
				posy = midposy + h;
				channelvertices.push_back(posx);
				channelvertices.push_back(posy);
				channelvertices.push_back(posz);
			}
			tempc = Collision(
				{ midposx - _r, midposy, midposz + _r }, { midposx + _r, midposy, midposz + _r },
				{ midposx + _r, midposy, midposz - _r }, { midposx - _r, midposy, midposz - _r },
				{ midposx - _r, midposy + h, midposz + _r }, { midposx + _r, midposy + h, midposz + _r },
				{ midposx + _r, midposy + h, midposz - _r }, { midposx - _r, midposy + h, midposz - _r });
			box.push_back(tempc);
		}
	}

	midposx = 500.0f;
	midposy = 0.0f;
	midposz = 500.0f;
	for (int k = 0; k <= 4; k++) {
		float theta = 2 * PI / 4.0f * (float)k;
		posx = std::cos(theta) * 30.0f + midposx;
		posy = midposy;
		posz = std::sin(theta) * 30.0f + midposz;
		channelvertices.push_back(posx);
		channelvertices.push_back(posy);
		channelvertices.push_back(posz);
		posy = midposy + 100.0f;
		channelvertices.push_back(posx);
		channelvertices.push_back(posy);
		channelvertices.push_back(posz);
	}
	tempc = Collision(
		{ midposx - 30.0f, midposy, midposz + 30.0f }, { midposx + 30.0f, midposy, midposz + 30.0f },
		{ midposx + 30.0f, midposy, midposz - 30.0f }, { midposx - 30.0f, midposy, midposz - 30.0f },
		{ midposx - 30.0f, midposy + 100.0f, midposz + 30.0f }, { midposx + 30.0f, midposy + 100.0f, midposz + 30.0f },
		{ midposx + 30.0f, midposy + 100.0f, midposz - 30.0f }, { midposx - 30.0f, midposy + 100.0f, midposz - 30.0f });
	box.push_back(tempc);

	midposx = -500.0f;
	midposy = 0.0f;
	midposz = -500.0f;
	for (int k = 0; k <= 6; k++) {
		float theta = 2 * PI / 6.0f * (float)k;
		posx = std::cos(theta) * 40.0f + midposx;
		posy = midposy;
		posz = std::sin(theta) * 40.0f + midposz;
		channelvertices.push_back(posx);
		channelvertices.push_back(posy);
		channelvertices.push_back(posz);
		posx = std::cos(theta) * 25.0f + midposx;
		posy = midposy + 100.0f;
		posz = std::sin(theta) * 25.0f + midposz;
		channelvertices.push_back(posx);
		channelvertices.push_back(posy);
		channelvertices.push_back(posz);
	}
	tempc = Collision(
		{ midposx - 40.0f, midposy, midposz + 40.0f }, { midposx + 40.0f, midposy, midposz + 40.0f },
		{ midposx + 40.0f, midposy, midposz - 40.0f }, { midposx - 40.0f, midposy, midposz - 40.0f },
		{ midposx - 40.0f, midposy + 100.0f, midposz + 40.0f }, { midposx + 40.0f, midposy + 100.0f, midposz + 40.0f },
		{ midposx + 40.0f, midposy + 100.0f, midposz - 40.0f }, { midposx - 40.0f, midposy + 100.0f, midposz - 40.0f });
	box.push_back(tempc);

	midposx = -500.0f;
	midposy = 0.0f;
	midposz = 500.0f;
	for (int k = 0; k <= segment; k++) {
		float theta = 2 * PI / (float)segment * (float)k;
		posx = std::cos(theta) * 40.0f + midposx;
		posy = midposy;
		posz = std::sin(theta) * 40.0f + midposz;
		channelvertices.push_back(posx);
		channelvertices.push_back(posy);
		channelvertices.push_back(posz);
		posx = midposx;
		posy = midposy + 100.0f;
		posz = midposz;
		channelvertices.push_back(posx);
		channelvertices.push_back(posy);
		channelvertices.push_back(posz);
	}
	tempc = Collision(
		{ midposx - 40.0f, midposy, midposz + 40.0f }, { midposx + 40.0f, midposy, midposz + 40.0f },
		{ midposx + 40.0f, midposy, midposz - 40.0f }, { midposx - 40.0f, midposy, midposz - 40.0f },
		{ midposx - 40.0f, midposy + 100.0f, midposz + 40.0f }, { midposx + 40.0f, midposy + 100.0f, midposz + 40.0f },
		{ midposx + 40.0f, midposy + 100.0f, midposz - 40.0f }, { midposx - 40.0f, midposy + 100.0f, midposz - 40.0f });
	box.push_back(tempc);

	midposx = 500.0f;
	midposy = 0.0f;
	midposz = -500.0f;
	for (int k = 0; k <= 6; k++) {
		float theta = 2 * PI / 6.0f * (float)k;
		posx = std::cos(theta) * 30.0f + midposx;
		posy = midposy;
		posz = std::sin(theta) * 30.0f + midposz;
		channelvertices.push_back(posx);
		channelvertices.push_back(posy);
		channelvertices.push_back(posz);
		posy = midposy + 100.0f;
		channelvertices.push_back(posx);
		channelvertices.push_back(posy);
		channelvertices.push_back(posz);
	}

	tempc = Collision(
		{ midposx - 30.0f, midposy, midposz + 30.0f }, { midposx + 30.0f, midposy, midposz + 30.0f },
		{ midposx + 30.0f, midposy, midposz - 30.0f }, { midposx - 30.0f, midposy, midposz - 30.0f },
		{ midposx - 30.0f, midposy + 100.0f, midposz + 30.0f }, { midposx + 30.0f, midposy + 100.0f, midposz + 30.0f },
		{ midposx + 30.0f, midposy + 100.0f, midposz - 30.0f }, { midposx - 30.0f, midposy + 100.0f, midposz - 30.0f });
	box.push_back(tempc);

	midposx = 0.0f;
	midposy = 300.0f;
	midposz = 0.0f;
	for (int j = 0; j < segment; j++) {
		for (int k = 0; k <= segment; k++) {
			float fai = PI / (float)segment * (float)j;
			float theta = 2 * PI / (float)segment * (float)k;
			posx = std::cos(theta) * std::sin(fai) * 20.0f + midposx;
			posy = std::cos(fai) * 20.0f + midposy;
			posz = std::sin(theta) * std::sin(fai) * 20.0f + midposz;
			channelvertices.push_back(posx);
			channelvertices.push_back(posy);
			channelvertices.push_back(posz);
			fai = PI / (float)segment * (float)(j + 1);
			posx = std::cos(theta) * std::sin(fai) * 20.0f + midposx;
			posy = std::cos(fai) * 20.0f + midposy;
			posz = std::sin(theta) * std::sin(fai) * 20.0f + midposz;
			channelvertices.push_back(posx);
			channelvertices.push_back(posy);
			channelvertices.push_back(posz);
		}
	}
	tempc = Collision(
		{ midposx - 20.0f, midposy - 20.0f, midposz + 20.0f }, { midposx + 20.0f, midposy - 20.0f, midposz + 20.0f },
		{ midposx + 20.0f, midposy - 20.0f, midposz - 20.0f }, { midposx - 20.0f, midposy - 20.0f, midposz - 20.0f },
		{ midposx - 20.0f, midposy + 20.0f, midposz + 20.0f }, { midposx + 20.0f, midposy + 20.0f, midposz + 20.0f },
		{ midposx + 20.0f, midposy + 20.0f, midposz - 20.0f }, { midposx - 20.0f, midposy + 20.0f, midposz - 20.0f });
	box.push_back(tempc);

	/*3-数据处理*/
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	//生成并绑定球体的VAO和VBO
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	//将顶点数据绑定至当前默认的缓冲中
	glBufferData(GL_ARRAY_BUFFER, channelvertices.size() * sizeof(float), &channelvertices[0], GL_STATIC_DRAW);

	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//解绑VAO和VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

channel::~channel() {
	cleanup();
}

void channel::draw(const glm::mat4& projection, const glm::mat4& view) {
	//glDepthFunc(GL_LEQUAL);
	_shader.use();
	_shader.setMat4("projection", projection);
	_shader.setMat4("view", view);
	glBindVertexArray(_vao);
	glBindVertexArray(_vao);
	for (int i = 0; i < 18; i++) {
		glDrawArrays(GL_TRIANGLE_STRIP, 2 * i * (_segment + 1), 2 * (_segment + 1));
	}
	glDrawArrays(GL_TRIANGLE_STRIP, 36 * (_segment + 1), 10);
	glDrawArrays(GL_TRIANGLE_STRIP, 36 * (_segment + 1) + 10, 14);
	glDrawArrays(GL_TRIANGLE_STRIP, 36 * (_segment + 1) + 24, 2 * (_segment + 1));
	glDrawArrays(GL_TRIANGLE_STRIP, 38 * (_segment + 1) + 24, 14);
	glDrawArrays(GL_TRIANGLE_STRIP, 38 * (_segment + 1) + 38, 2 * _segment * (_segment + 1));
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

void channel::cleanup() {
	if (_vbo != 0) {
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
	}

	if (_vao != 0) {
		glDeleteVertexArrays(1, &_vao);
		_vao = 0;
	}
}