#pragma once

#include <memory>
#include <string>

#include "../base/application.h"
#include "../base/model.h"
#include "../base/light.h"
#include "../base/shader.h"
#include "../base/texture.h"
#include "../base/camera.h"
#include "../base/skybox.h"
#include "bunny.h"
#include "channel.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const unsigned int SHADOW_WIDTH = 1024 * 10;
const unsigned int SHADOW_HEIGHT = 1024 * 10;


class TextureMapping : public Application {
public:
	bool wireframe;
	bool camera_fixed;
	TextureMapping();
	~TextureMapping();

	GLuint depthMap;
	GLuint depthMapFBO;



private:
	std::unique_ptr<channel> _basic_voxel;
	std::unique_ptr<Model> _race_track;

	//postures
	std::vector<std::unique_ptr<Model>> _postures;
	std::vector<std::shared_ptr<Model>> unhit_ball;
	std::vector<std::shared_ptr<Model>> hit_ball;
	std::unique_ptr<Bunny> _bunny;

	//use for shadow
	std::unique_ptr<Shader> _simpleShader;
	std::unique_ptr<Shader> _lightShader;
	std::unique_ptr<Shader> _skyboxShader;
	std::unique_ptr<Shader> _shadowShader;
	std::unique_ptr<Shader> _simpleDepthShader;

	glm::vec3 _albedo = { 1.0f, 1.0f, 1.0f };
	float _ka = 0.041f;
	glm::vec3 _kd = { 1.0f, 1.0f, 1.0f };
	glm::vec3 _ks = { 1.0f, 1.0f, 1.0f };
	float _ns = 10.0f;
	float _intensity = 1.0f;

	std::unique_ptr<Texture> _blendTextures[4];

	std::unique_ptr<SkyBox> _skybox;
	std::unique_ptr<PerspectiveCamera> _active_camera;
	std::unique_ptr<PerspectiveCamera> _fixed_camera;
	std::unique_ptr<DirectionalLight> _light;
	

	void initSimpleShader();

	void initShadowShader();

	void initsimpleDepthShader();

	void initSkyboxShader();

	void initLightShader();

	void handleInput() override;

	void renderFrame() override;


};