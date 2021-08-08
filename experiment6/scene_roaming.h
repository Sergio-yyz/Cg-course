#pragma once

#include <memory>
#include <vector>

#include "../base/camera.h"
#include "../base/application.h"
#include "bunny.h"

class SceneRoaming : public Application {
public:
	SceneRoaming();
	
	~SceneRoaming();

	void importModel(const std::string& filepath);

	void handleInput() override;

	void renderFrame() override;

private:
	const float _cameraMoveSpeed = 5.0f;
	const float _cameraRotateSpeed = 0.2f;

	std::vector<std::shared_ptr<Camera>> _cameras;
	int activeCameraIndex = 0;

	std::shared_ptr<Bunny> _bunny;
};