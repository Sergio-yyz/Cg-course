#pragma once

#include <vector>
#include <queue>

#include "../base/vertex.h"
#include "../base/shader.h"
#include "../base/object3d.h"
#include "../base/model.h"

enum Direction {
	bunny_FORWARD,
	bunny_BACKWARD,
	bunny_LEFT,
	bunny_RIGHT
};

class Bunny :public Model {
public:
	Bunny(const std::string modelPath);
	glm::mat4 getModelMatrix() const override;
	bool checkCollision(Collision kernel2);

	//------------motions of the bunny-----------------
	int ready = 0;
	int started = 0;
	int checkpoints = 0;
	int ended = 0;
	clock_t starttime = 0;
	clock_t endtime = 0;

	float Yaw;
	glm::vec3 front;
	std::queue<float> HistoryYaw;
	int DelayFrameNum = 20;
	float DelayYaw;

	std::queue<glm::vec3> HistoryPosition;
	glm::vec3 DelayPosition;

	float MovementSpeed;
	float TurningSpeed;

	int Inertia(float deltaTime);
	void ProcessKey(Direction d, float deltatime, bool isbounce);

	void UpdateDelayYaw();
	void UpdateDelayPosition();
	void UpdateRotation();
	void Check();
	void Restart();


};