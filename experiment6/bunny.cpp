#include "bunny.h"

GLfloat checkmid1[] = {
	-378.917f, 371.533f,
	-149.387f, 193.246f,
	161.782f,  337.101f,
	93.352f,   -59.467f,
	389.134f, -192.771f,
	15.901f,  -183.755f,
	-210.638f,-399.19f,
	-77.433f, -85.107f,
	-210.272f, 110.339f
};

Bunny::Bunny(const std::string modelPath) : Model(modelPath), MovementSpeed(90.0), TurningSpeed(90.0), Yaw(0.0), DelayYaw(0.0), front(glm::vec3(0.0, 0.0, -1.0)) {

}

void Bunny::UpdateDelayYaw()
{
	HistoryYaw.push(Yaw);
	while (HistoryYaw.size() > DelayFrameNum) {
		HistoryYaw.pop();
	}
	DelayYaw = HistoryYaw.front();
}

void Bunny::UpdateDelayPosition()
{
	HistoryPosition.push(position);
	while (HistoryPosition.size() > DelayFrameNum) {
		HistoryPosition.pop();
	}
	DelayPosition = HistoryPosition.front();
}

void Bunny::UpdateRotation()
{
	glm::vec3 f = glm::vec3(1.0f);
	f.x = -sin(glm::radians((DelayYaw + Yaw) / 2));
	f.y = 0.0f;
	f.z = -cos(glm::radians((DelayYaw + Yaw) / 2));
	front = glm::normalize(f);
}

int Bunny::Inertia(float deltaTime) {
	
	MovementSpeed = 0.0f;
	position += front * MovementSpeed * deltaTime;
	return 0;
}

void Bunny::ProcessKey(Direction d, float deltaTime, bool isbounce)
{
	int bounce = (isbounce == false) ? 1 : -5;
	if (d == bunny_FORWARD) {
		position += front * MovementSpeed * deltaTime * bounce ;
	}
	if (d == bunny_BACKWARD) {
		position -= front * MovementSpeed * deltaTime * bounce ;
	}
	if (d == bunny_LEFT)
		Yaw += TurningSpeed * deltaTime * bounce ;
	if (d == bunny_RIGHT)
		Yaw -= TurningSpeed * deltaTime * bounce ;
	UpdateRotation();
	Check();
}

glm::mat4 Bunny::getModelMatrix() const {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, (DelayPosition + position) / 2.0f + glm::vec3(0.0, 2.8, 0.0));
	model = glm::rotate(model, glm::radians((DelayYaw) / 2.0f), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(Yaw - DelayYaw / 2.0f), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
	model = glm::scale(model, glm::vec3(0.04, 0.04, 0.04));
	return model;
}

bool InBox(float x1, float y1, float x2, float y2, float x, float y) {
	float x_min = (x1 >= x2) ? x2 : x1;
	float x_max = (x1 >= x2) ? x1 : x2;
	float y_min = (y1 >= y2) ? y2 : y1;
	float y_max = (y1 >= y2) ? y1 : y2;
	if (x_min <= x && x <= x_max && y_min < y && y <= y_max) return true;
	else return false;
}

bool Bunny::checkCollision(Collision kernel2) {
	Collision kernel1 = this->kernel;
	bool xy_collision = false;
	bool z_collision = false;
	glm::mat4 Model1 = this->getModelMatrix();
	glm::vec4 p1_1 = Model1 * glm::vec4(kernel1.r_f_d, 1.0f);
	glm::vec4 p1_2 = Model1 * glm::vec4(kernel1.r_b_t, 1.0f);
	glm::vec4 p2_1 = glm::vec4(kernel2.l_f_d, 1.0f);
	glm::vec4 p2_2 = glm::vec4(kernel2.r_b_d, 1.0f);

	float x1 = p1_1.x;
	float y1 = p1_1.z;
	float x2 = p1_2.x;
	float y2 = p1_2.z;


	std::vector <glm::vec4> p;
	glm::vec4 p2;
	glm::vec4 p1;

	p.clear();
	p2 = glm::vec4(kernel2.l_f_d, 1.0f); p.push_back(p2);
	p2 = glm::vec4(kernel2.l_b_d, 1.0f); p.push_back(p2);
	p2 = glm::vec4(kernel2.r_f_d, 1.0f); p.push_back(p2);
	p2 = glm::vec4(kernel2.r_b_d, 1.0f); p.push_back(p2);
	for (const auto& pp : p) {
		if (InBox(x1, y1, x2, y2, pp.x, pp.z)) {
			xy_collision = true;
			break;
		}
	}

	x1 = p2_1.x;
	y1 = p2_1.z;
	x2 = p2_2.x;
	y2 = p2_2.z;
	p.clear();
	p1 = Model1 * glm::vec4(kernel1.r_f_d, 1.0f); p.push_back(p1);
	p1 = Model1 * glm::vec4(kernel1.r_b_d, 1.0f); p.push_back(p1);
	p1 = Model1 * glm::vec4(kernel1.r_b_t, 1.0f); p.push_back(p1);
	p1 = Model1 * glm::vec4(kernel1.r_f_t, 1.0f); p.push_back(p1);
	for (const auto& pp : p) {
		if (InBox(x1, y1, x2, y2, pp.x, pp.z)) {
			xy_collision = true;
			break;
		}
	}

	if (!xy_collision) return false;
	else {
		p1_1 = Model1 * glm::vec4(kernel1.l_f_d, 1.0f);
		p1_2 = Model1 * glm::vec4(kernel1.r_f_d, 1.0f);
		float z1_1 = p1_1.y;
		float z1_2 = p1_2.y;
		p1_1 = glm::vec4(kernel2.l_f_d, 1.0f);
		p1_2 = glm::vec4(kernel2.l_f_t, 1.0f);
		float z2_1 = p1_1.y;
		float z2_2 = p1_2.y;
		if ((z2_1 <= z1_1 && z2_2 >= z1_1) || (z2_1 <= z1_2 && z2_2 >= z1_2)) z_collision = true;
	}
	return z_collision;
}


void Bunny::Check() {
	if (!started)return;
	float checkx, checkz;
	checkx = checkmid1[checkpoints * 2];
	checkz = checkmid1[checkpoints * 2 + 1];
	if ((position.x - checkx) * (position.x - checkx) + (position.z - checkz) * (position.z - checkz) < 25.0f * 25.0f) checkpoints++;
	if (checkpoints == 9) ended = 1;
}

void Bunny::Restart() {
	position = { -210.272f, 0.0f, 110.339f };
	Yaw = 136.77f;
	checkpoints = 0;
	ended = 0;
}