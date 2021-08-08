#include <unordered_map>
#include <windows.h>
#pragma comment(lib,"winmm.lib")
#include<mmsystem.h>

#include "tiny_obj_loader.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "screen_shot.h"
#include "texture_mapping.h"

const std::string ballPath = "../data/sphere.obj";
const std::string trackPath = "../data/race-track/race-track.obj";
const std::string carPath = "../data/2DoorCoupe/10611_2DoorCoupe.obj";
const std::string blendTexturePaths[4] = {
	"../data/race-track/racing-track.png", 
	"../data/2DoorCoupe/CoupeC.jpg",
	"../data/earthmap.jpg",
	"../data/postures/peasant_girl_diffuse.png"
};

const std::vector<std::string> skyboxTexturePaths = {
	"../data/skybox/right.jpg",
	"../data/skybox/left.jpg",
	"../data/skybox/bottom.jpg",
	"../data/skybox/top.jpg",
	"../data/skybox/front.jpg",
	"../data/skybox/back.jpg"
};

TextureMapping::TextureMapping() {
	_windowTitle = "Texture Mapping";

	mciSendString(TEXT("open \"../data/3770199609.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("play mp3 repeat"), NULL, 0, NULL);
	//basic voxel
	_basic_voxel.reset(new channel(10.0f, 1.5f, 20.0f));

	// init model

	//ball
	std::vector<glm::vec3> ball_pos;
	std::shared_ptr<Model> ball;

	ball_pos.clear();
	ball_pos.push_back(glm::vec3(100.0f, 10.0f, 100.0f));
	ball_pos.push_back(glm::vec3(-100.0f, 10.0f, 100.0f));
	ball_pos.push_back(glm::vec3(100.0f, 10.0f, -100.0f));
	ball_pos.push_back(glm::vec3(-100.0f, 10.0f, -100.0f));
	ball_pos.push_back(glm::vec3(-299.9f, 10.0f, 205.7f));
	ball_pos.push_back(glm::vec3(-378.1f, 10.0f, 365.8f));
	ball_pos.push_back(glm::vec3(-169.2f, 10.0f, 225.6f));
	ball_pos.push_back(glm::vec3(-75.2f, 10.0f, 181.6f));
	ball_pos.push_back(glm::vec3(38.5f, 10.0f, 273.9f));
	ball_pos.push_back(glm::vec3(164.1f, 10.0f, 347.1f));
	ball_pos.push_back(glm::vec3(191.5f, 10.0f, 233.1f));
	ball_pos.push_back(glm::vec3(93.1f, 10.0f, 14.4f));
	ball_pos.push_back(glm::vec3(175.9f, 10.0f, -110.9f));
	ball_pos.push_back(glm::vec3(277.4f, 10.0f, -135.8f));
	ball_pos.push_back(glm::vec3(395.3f, 10.0f, -199.1f));
	ball_pos.push_back(glm::vec3(345.5f, 10.0f, -292.6f));
	ball_pos.push_back(glm::vec3(260.3f, 10.0f, -271.3f));
	ball_pos.push_back(glm::vec3(142.2f, 10.0f, -218.5f));
	ball_pos.push_back(glm::vec3(56.0f, 10.0f, -188.6f));
	ball_pos.push_back(glm::vec3(-42.9f, 10.0f, -220.7f));
	ball_pos.push_back(glm::vec3(-91.2f, 10.0f, -349.4f));
	ball_pos.push_back(glm::vec3(-189.9f, 10.0f, -417.3f));
	ball_pos.push_back(glm::vec3(-259.5f, 10.0f, -350.3f));
	ball_pos.push_back(glm::vec3(-218.8f, 10.0f, -262.2f));
	ball_pos.push_back(glm::vec3(-143.5f, 10.0f, -150.6f));
	ball_pos.push_back(glm::vec3(-80.5f, 10.0f, -76.35f));
	ball_pos.push_back(glm::vec3(-135.0f, 10.0f, 43.0f));
	ball_pos.push_back(glm::vec3(-202.2f, 10.0f, 111.1f));
	for (const auto& p : ball_pos) {
		ball.reset(new Model(ballPath));
		ball->scale = glm::vec3(5.0f, 5.0f, 5.0f);
		ball->position = p;
		hit_ball.push_back(ball);
	}

	ball_pos.clear();
	ball_pos.push_back(glm::vec3(75.0f, 45.0f, 75.0f));
	ball_pos.push_back(glm::vec3(-75.0f, 40.0f, 75.0f));
	ball_pos.push_back(glm::vec3(-75.2f, 50.0f, 181.6f));
	ball_pos.push_back(glm::vec3(75.0f, 30.0f, -75.0f));
	ball_pos.push_back(glm::vec3(-75.0f, 35.0f, -75.0f));
	ball_pos.push_back(glm::vec3(277.4f, 55.0f, -135.8f));
	ball_pos.push_back(glm::vec3(-189.9f, 50.0f, -417.3f));
	ball_pos.push_back(glm::vec3(164.1f, 50.0f, 347.1f));
	ball_pos.push_back(glm::vec3(-218.8f, 50.0f, -262.2f));
	ball_pos.push_back(glm::vec3(-299.9f, 50.0f, 205.7f));
	ball_pos.push_back(glm::vec3(260.3f, 50.0f, -271.3f));
	for (const auto& p : ball_pos) {
		ball.reset(new Model(ballPath));
		ball->scale = glm::vec3(5.0f, 5.0f, 5.0f);
		ball->position = p;
		unhit_ball.push_back(ball);
	}

	//init track
	_race_track.reset(new Model(trackPath));
	_race_track->scale = glm::vec3(3.0f, 3.0f, 3.0f);

	//init posture
	std::string prefix = "../data/postures/pose";
	std::string postfix = ".obj";
	std::string zero = "0";
	std::vector<std::string> middle;
	for (int i = 0; i <= 100; i++)
	{
		_postures.push_back(std::unique_ptr<Model>());
		std::string tempstring = std::to_string(i);
		std::string resultpath;
		if (i >= 0 && i <= 9)
		{
			resultpath.append(prefix);
			resultpath.append(zero);
			resultpath.append(zero);
			resultpath.append(tempstring);
			resultpath.append(postfix);
		}
		else if (i >= 10 && i <= 99)
		{
			resultpath.append(prefix);
			resultpath.append(zero);
			resultpath.append(tempstring);
			resultpath.append(postfix);
		}
		else
		{
			resultpath.append(prefix);
			resultpath.append(tempstring);
			resultpath.append(postfix);
		}
		_postures[i].reset(new Model(resultpath));
	}
	
	// init skybox
	_skybox.reset(new SkyBox(skyboxTexturePaths));

	// init camera
	_active_camera.reset(new PerspectiveCamera(glm::radians(50.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, 10000.0f));
	_active_camera->position.z = 10.0f;
	_active_camera->is_fixed = false;
	_fixed_camera.reset(new PerspectiveCamera(glm::radians(50.0f), 1.0f * _windowWidth / _windowHeight, 0.1f, 10000.0f));
	_fixed_camera->position.z = 10.0f;
	_fixed_camera->is_fixed = true;
	//_fixed_camera->is_fixed = false;
	_fixed_camera->front = glm::vec3(0.0, 0.0, 1.0);

	_bunny.reset(new Bunny(carPath));
	_bunny->Restart();
	_bunny->UpdateDelayYaw();
	_bunny->UpdateRotation();
	_fixed_camera->is_orbitting = false;
	_fixed_camera->zoomOut();

	// init light
	_light.reset(new DirectionalLight());
	_light->rotation = glm::angleAxis(glm::radians(90.0f), -glm::vec3(1.0f, 1.0f, 1.0f));
	_light->position = glm::vec3(-1000.0f, 1000.0f, -1000.0f);

	// init textures
	for (int i = 0; i < 4; ++i) {
		_blendTextures[i].reset(new Texture2D(blendTexturePaths[i]));
	}



	wireframe = false;
	camera_fixed = false;

	// init shaders
	initLightShader();
	initSimpleShader();
	initSkyboxShader();
	initShadowShader();
	initsimpleDepthShader(); 


	_shadowShader->use();
	_shadowShader->setInt("diffuseTexture", 0);
	_shadowShader->setInt("shadowMap", 10);


	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init();
}

TextureMapping::~TextureMapping() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


void TextureMapping::initLightShader() {
	const char* vertCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"out vec3 worldPosition;\n"
		"out vec3 normal;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main() {\n"
		"	normal = mat3(transpose(inverse(model))) * aNormal;\n"
		"	worldPosition = vec3(model * vec4(aPosition, 1.0f));\n"
		"	gl_Position = projection * view * vec4(worldPosition, 1.0f);\n"
		"}\n";

	const char* fragCode =
		"#version 330 core\n"
		"in vec3 worldPosition;\n"
		"in vec3 normal;\n"
		"out vec4 fragColor;\n"
		"void main() {\n"
		"vec3 lightPosition = vec3(100.0f, 100.0f, 100.0f);\n"
		"// ambient color\n"
		"float ka = 0.6f;\n"
		"vec3 objectColor = vec3(1.0f, 1.0f, 1.0f);\n"
		"vec3 ambient = ka * objectColor;\n"
		"// diffuse color\n"
		"float kd = 0.8f;\n"
		"vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);\n"
		"vec3 lightDirection = normalize(lightPosition - worldPosition);\n"
		"vec3 diffuse = kd * lightColor * max(dot(normalize(normal), lightDirection), 0.0f);\n"
		"fragColor = vec4(ambient + diffuse, 1.0f) * vec4(objectColor,1.0f); \n"
		"}\n";
	_lightShader.reset(new Shader(vertCode, fragCode));
}

void TextureMapping::initSimpleShader() {
	const char* vertCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoord;\n"
		"out vec2 TexCoord;\n"
		"uniform mat4 projection;\n"
		"uniform mat4 view;\n"
		"uniform mat4 model;\n"

		"void main() {\n"
		"	TexCoord = aTexCoord;\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
		"}\n";

	const char* fragCode =
		"#version 330 core\n"
		"in vec2 TexCoord;\n"
		"out vec4 color;\n"
		"uniform sampler2D mapKd;\n"
		"void main() {\n"
		"	color = texture(mapKd, TexCoord);\n"
		"}\n";

	_simpleShader.reset(new Shader(vertCode, fragCode));
}

void TextureMapping::initShadowShader() {
	const char* vertCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoords;\n"

		"out vec2 TexCoords;\n"

		"out VS_OUT{\n"
		"vec3 FragPos;\n"
		"vec3 Normal;\n"
		"vec2 TexCoords;\n"
		"vec4 FragPosLightSpace;\n"
		"} vs_out;\n"

		"uniform mat4 projection;\n"
		"uniform mat4 view;\n"
		"uniform mat4 model;\n"
		"uniform mat4 lightSpaceMatrix;\n"

		"void main()\n"
		"{\n"
		"vs_out.FragPos = vec3(model * vec4(aPos, 1.0));\n"
		"vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;\n"
		"vs_out.TexCoords = aTexCoords;\n"
		"vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);\n"
		"gl_Position = projection * view * model * vec4(aPos, 1.0);\n"

		"}\n";
	
	const char* fragCode =
		"#version 330 core\n"
		"in VS_OUT{\n"
		"vec3 FragPos;\n"
		"vec3 Normal;\n"
		"vec2 TexCoords;\n"
		"vec4 FragPosLightSpace;\n"
		"} fs_in;\n"

		"out vec4 FragColor;\n"

		"struct Material {\n"
		"	vec3 albedo;\n"
		"	float ka;\n"
		"	vec3 kd;\n"
		"   vec3 ks;\n"
		"   float ns;\n"
		"};\n"
		"uniform float intensity;"

		"uniform Material material;\n"
		"uniform sampler2D diffuseTexture;\n"
		"uniform sampler2D shadowMap;\n"
		"uniform vec3 lightPos;\n"
		"uniform vec3 viewPos;\n"

		"float ShadowCalculation(vec4 fragPosLightSpace)\n"
		"{\n"
		"vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;\n"
		"vec3 lightDir = normalize(lightPos - fs_in.FragPos);\n"
		"projCoords = projCoords * 0.5 + 0.5;\n"
		"float closestDepth = texture(shadowMap, projCoords.xy).r;\n"
		"float currentDepth = projCoords.z;\n"
		"vec3 normal = normalize(fs_in.Normal);\n"
		"float bias = max(0.001 * (1.0 - dot(normal, lightDir)), 0.00001);\n"
		"float shadow = 0.0;\n"
		"vec2 texelSize = 1.0 / textureSize(shadowMap, 0);\n"
		"for (int x = -1; x <= 1; ++x) {\n"
		"for (int y = -1; y <= 1; ++y) {\n"
		"float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;\n"
		"shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;\n"
		"}\n"
		"}\n"
		"shadow = shadow / 9.0;\n"
		"if (projCoords.z > 1.0)\n"
		"shadow = 0.0;\n"
		"return shadow; \n"
		"}\n"

		"void main()\n"
		"{\n"
		"vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;\n"
		"vec3 normal = normalize(fs_in.Normal);\n"
		"vec3 lightColor = vec3(0.5);\n"
		//ambient
		"vec3 ambient = material.ka * material.albedo;\n"
		//diffuse
		"vec3 lightDir = normalize(lightPos - fs_in.FragPos);\n"
		"float diff = max(dot(lightDir, normal), 0.0);\n"
		"vec3 diffuse = diff * color * material.kd * intensity;\n"
		//specular
		"vec3 viewDir = normalize(viewPos - fs_in.FragPos);\n"
		"vec3 reflectDir = reflect(-lightDir, normal);\n"
		"vec3 halfwayDir = normalize(lightDir + viewDir);\n"
		"vec3 spec = pow(max(dot(normal, halfwayDir), 0.0), material.ns) * material.ks;\n"
		"vec3 specular = spec * lightColor * intensity;\n"

		"float shadow = ShadowCalculation(fs_in.FragPosLightSpace);\n"
		"vec3 lighting = (ambient + (1.0 - shadow ) * (diffuse + specular)) * color;\n"
		"FragColor = vec4(lighting, 1.0);\n"
		"}\n";
	_shadowShader.reset(new Shader(vertCode, fragCode));
}

void TextureMapping::initsimpleDepthShader() {
	const char* vertCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"

		"uniform mat4 lightSpaceMatrix;\n"
		"uniform mat4 model;\n"

		"void main()\n"
		"{\n"
		"gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);\n"
		"}\n";
	const char* fragCode =
		"#version 330 core\n"
		"void main()\n"
		"{\n"
		//"gl_FragDepth = gl_FragCoord.z;	
		"}\n";

	_simpleDepthShader.reset(new Shader(vertCode, fragCode));
}


void TextureMapping::initSkyboxShader() {
	const char* vertCode = 
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"out vec3 texCoord;\n"
		"uniform mat4 projection;\n"
		"uniform mat4 view;\n"
		"void main() {\n"
		"   texCoord = vec3(aPosition.x, -aPosition.y, aPosition.z);\n"
		"   vec4 pos = projection * view * vec4(aPosition, 1.0f);\n"
		"   gl_Position = pos.xyww;\n"
		"}\n";

	const char* fragCode = 
		"#version 330 core\n"
		"out vec4 color;\n"
		"in vec3 texCoord;\n"
		"uniform samplerCube cubemap;\n"
		"void main() {\n"
		"   color = texture(cubemap, texCoord);\n"
		"}\n";
	_skyboxShader.reset(new Shader(vertCode, fragCode));
}
void TextureMapping::handleInput() {
	const float angluarVelocity = 0.1f;
	const float angle = angluarVelocity * static_cast<float>(_deltaTime);
	const glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);

	if (_keyboardInput.keyStates[GLFW_KEY_ESCAPE] != GLFW_RELEASE) {
		glfwSetWindowShouldClose(_window, true);
		return;
	}

	if (_keyboardInput.keyStates[GLFW_KEY_W] != GLFW_RELEASE) {
		std::cout << "W" << std::endl;
		/* write your code here */
		// move front
		_active_camera->position += _active_camera->getFront();
	}

	if (_keyboardInput.keyStates[GLFW_KEY_A] != GLFW_RELEASE) {
		std::cout << "A" << std::endl;
		/* write your code here */
		// move left
		_active_camera->position -= _active_camera->getRight();
		_fixed_camera->is_orbitting = true;
		if (_fixed_camera->is_fixed&& _fixed_camera->is_orbitting) {
			
			_fixed_camera->orbit((_bunny->DelayPosition + _bunny->position) / 2.0f + glm::vec3(0.0, 10.0, 0.0),1,_deltaTime);
		}
	}

	if (_keyboardInput.keyStates[GLFW_KEY_S] != GLFW_RELEASE) {
		std::cout << "S" << std::endl;
		_active_camera->position -= _active_camera->getFront();
	}

	if (_keyboardInput.keyStates[GLFW_KEY_D] != GLFW_RELEASE) {
		std::cout << "D" << std::endl;
		_fixed_camera->is_orbitting = true;
		_active_camera->position += _active_camera->getRight();
		if (_fixed_camera->is_fixed && _fixed_camera->is_orbitting) {
			_fixed_camera->orbit((_bunny->DelayPosition + _bunny->position) / 2.0f + glm::vec3(0.0, 10.0, 0.0), -1, _deltaTime);
		}
	}

	if (_keyboardInput.keyStates[GLFW_KEY_R] == GLFW_PRESS) {
		std::cout << "R" << std::endl;
		_bunny->Restart();
		_bunny->ready = 1;
		_bunny->started = 0;
		_bunny->starttime = clock();
		_active_camera->zoomOut();
		_keyboardInput.keyStates[GLFW_KEY_R] = GLFW_RELEASE;
		return;
	}

	if (_keyboardInput.keyStates[GLFW_KEY_I] != GLFW_RELEASE) {
		std::cout << "I" << std::endl;
		_active_camera->position += glm::vec3(0.0f, 0.1f, 0.0f);
	}

	if (_keyboardInput.keyStates[GLFW_KEY_K] != GLFW_RELEASE) {
		std::cout << "K" << std::endl;
		_active_camera->position += glm::vec3(0.0f, -0.1f, 0.0f);
	}

	if (_keyboardInput.keyStates[GLFW_KEY_Z] != GLFW_RELEASE) {
		std::cout << "Z" << std::endl;
		_active_camera->zoomIn();
	}

	if (_keyboardInput.keyStates[GLFW_KEY_C] != GLFW_RELEASE) {
		std::cout << "C" << std::endl;
		_active_camera->zoomOut();
	}

	if (_keyboardInput.keyStates[GLFW_KEY_Q] == GLFW_PRESS) {
		std::cout << "Q" << std::endl;
		/* write your code here */
		// move up
		wireframe = !wireframe;
		_keyboardInput.keyStates[GLFW_KEY_Q] = GLFW_RELEASE;
		return;
	}

	if (_keyboardInput.keyStates[GLFW_KEY_P] == GLFW_PRESS) {
		std::cout << "P" << std::endl;
		/* write your code here */
		// move up
		screen_shot();
		std::cout<<"car position: " << _bunny->position.x <<","<<_bunny->position.y<<","<<_bunny->position.z<< std::endl;
		_keyboardInput.keyStates[GLFW_KEY_P] = GLFW_RELEASE;
		return;
	}

	if (_keyboardInput.keyStates[GLFW_KEY_E] == GLFW_PRESS) {
		std::cout << "E" << std::endl;
		/* write your code here */
		// move down
		_fixed_camera->is_fixed = !_fixed_camera->is_fixed;
		_keyboardInput.keyStates[GLFW_KEY_E] = GLFW_RELEASE;
		return;
	}

	if (_keyboardInput.keyStates[GLFW_KEY_X] == GLFW_PRESS) {
		std::cout << "X" << std::endl;
		/* write your code here */
		// move down
		_active_camera->zoomFit();
		_keyboardInput.keyStates[GLFW_KEY_X] = GLFW_RELEASE;
		return;
	}


	bool isCollision = false;
	for (const auto& box : _basic_voxel->box) {
		if (_bunny->checkCollision(box)) {
			isCollision = true;
			//std::cout << "jb" << std::endl;
			break;
		}
	}

	if (_keyboardInput.keyStates[GLFW_KEY_UP] != GLFW_RELEASE) {
		if (_bunny->started == 1) {
			std::cout << "UP" << std::endl;
			/* write your code here */
			// move front
			_fixed_camera->is_orbitting = false;
			_bunny->ProcessKey(bunny_FORWARD, _deltaTime, isCollision);
			_fixed_camera->zoomOut();
			if (_keyboardInput.keyStates[GLFW_KEY_LEFT] != GLFW_RELEASE) {
				std::cout << "LEFT" << std::endl;
				_bunny->ProcessKey(bunny_LEFT, _deltaTime, isCollision);
			}
			if (_keyboardInput.keyStates[GLFW_KEY_RIGHT] != GLFW_RELEASE) {
				std::cout << "RIGHT" << std::endl;
				_bunny->ProcessKey(bunny_RIGHT, _deltaTime, isCollision);
			}
		}
		//std::cout << _bunny->position.x << _bunny->position.y << _bunny->position.z << std::endl;

	}
	if (_keyboardInput.keyStates[GLFW_KEY_DOWN] != GLFW_RELEASE) {
		if (_bunny->started == 1) {
			std::cout << "DOWN" << std::endl;
			/* write your code here */
			// move left
			_fixed_camera->is_orbitting = false;
			_bunny->ProcessKey(bunny_BACKWARD, _deltaTime, isCollision);
			_fixed_camera->zoomIn();
			if (_keyboardInput.keyStates[GLFW_KEY_LEFT] != GLFW_RELEASE) {
				std::cout << "LEFT" << std::endl;
				_bunny->ProcessKey(bunny_LEFT, _deltaTime, isCollision);
			}
			if (_keyboardInput.keyStates[GLFW_KEY_RIGHT] != GLFW_RELEASE) {
				std::cout << "RIGHT" << std::endl;
				_bunny->ProcessKey(bunny_RIGHT, _deltaTime, isCollision);
			}
		}

	}


	if (_mouseInput.move.xCurrent != _mouseInput.move.xOld) {
		std::cout << "mouse move in x direction" << std::endl;
		// rotate around world up: glm::vec3(0.0f, 1.0f, 0.0f)
		const float angle = -1 * _deltaTime * (_mouseInput.move.xCurrent - _mouseInput.move.xOld);
		const glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
		/* write your code here */
		// you should know how quaternion works to represent rotation
		//float theta = glm::acos(_camera->rotation.w) * 2.0;
		//_camera->rotation = glm::angleAxis(glm::radians(angle), axis)*_camera->rotation;
		_active_camera->rotation = glm::quat(cos(angle / 2.0), axis.x * sin(angle / 2.0), axis.y * sin(angle / 2.0), axis.z * sin(angle / 2.0)) * _active_camera->rotation;
		_mouseInput.move.xOld = _mouseInput.move.xCurrent;
	}

	if (_mouseInput.move.yCurrent != _mouseInput.move.yOld) {
		std::cout << "mouse move in y direction" << std::endl;
		/* write your code here */
		// rotate around local right
		const float angle = -1 * _deltaTime * (_mouseInput.move.yCurrent - _mouseInput.move.yOld);
		const glm::vec3 axis = _active_camera->getRight();
		// you should know how quaternion works to represent rotation
		//_camera->rotation = glm::angleAxis(glm::radians(angle), axis)*_camera->rotation;
		_active_camera->rotation = glm::quat(cos(angle / 2.0), axis.x * sin(angle / 2.0), axis.y * sin(angle / 2.0), axis.z * sin(angle / 2.0)) * _active_camera->rotation;
		_mouseInput.move.yOld = _mouseInput.move.yCurrent;
	}
	

}

void TextureMapping::renderFrame() {

	
	// trivial things
	showFpsInWindowTitle();

	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if (wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glm::mat4 projection;
	glm::mat4 shader_projection;
	glm::mat4 view;
	glm::vec3 viewpos;

	if (_fixed_camera->is_fixed) {
		if (!_fixed_camera->is_orbitting) {
			_fixed_camera->fix_camera((_bunny->DelayPosition + _bunny->position) / 2.0f + glm::vec3(0.0, 10.0, 0.0) - 20 * _bunny->front, _bunny->front);
		}
		shader_projection = _fixed_camera->getProjectionMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
		projection = _fixed_camera->getProjectionMatrix();
		view = _fixed_camera->getViewMatrix();
		viewpos = _fixed_camera->position;
	}
	else {
		_fixed_camera->is_orbitting = false;
		shader_projection = _active_camera->getProjectionMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
		projection = _active_camera->getProjectionMatrix();
		view = _active_camera->getViewMatrix();
		viewpos = _active_camera->position;
	}
	
	for (auto p = hit_ball.begin(); p != hit_ball.end(); ++p) {
		if (_bunny->checkCollision((*p)->getWorldKernel())) {
			hit_ball.erase(p);
			break;
		}
	}

		glm::mat4 lightView = glm::lookAt(_light->position, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 lightProjection = glm::ortho(
			-2000.0f, 2000.0f,
			-2000.0f, 2000.0f,
			-2000.0f, 2000.0f);
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		_simpleDepthShader->use();
		_simpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		//race track
		glActiveTexture(GL_TEXTURE0);
		_blendTextures[0]->bind();
		_simpleDepthShader->setMat4("model", _race_track->getModelMatrix());
		_race_track->draw();
		_blendTextures[0]->unbind();

		//postures
		static int i;
		i++;
		int j = (i % 1000) / 10;
		glActiveTexture(GL_TEXTURE0);
		_blendTextures[3]->bind();
		_simpleDepthShader->setMat4("model", _postures[j]->getModelMatrix() * glm::scale(glm::mat4(1.0), glm::vec3(0.04, 0.04, 0.04)));
		_postures[j]->draw();
		_blendTextures[3]->unbind();

		//car
		glActiveTexture(GL_TEXTURE0);
		_blendTextures[1]->bind();
		_simpleDepthShader->setMat4("model", _bunny->getModelMatrix());
		_bunny->draw();
		_blendTextures[1]->unbind();

		//hitable ball
		for (const auto& ball : hit_ball) {
			glActiveTexture(GL_TEXTURE0);
			_blendTextures[2]->bind();
			_simpleDepthShader->setMat4("model", ball->getModelMatrix());
			ball->draw();
			_blendTextures[2]->unbind();
		}

		//unhitable ball
		for (const auto& ball : unhit_ball) {
			glActiveTexture(GL_TEXTURE0);
			_blendTextures[2]->bind();
			_simpleDepthShader->setMat4("model", ball->getModelMatrix());
			ball->draw();
			_blendTextures[2]->unbind();
		}
		
		_bunny->UpdateDelayPosition();
		_bunny->UpdateDelayYaw();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		_shadowShader->use();
		_shadowShader->setVec3("viewPos", viewpos);
		_shadowShader->setVec3("lightPos", _light->position);
		_shadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		_shadowShader->setMat4("projection", shader_projection);
		_shadowShader->setMat4("view", view);
		_shadowShader->setVec3("material.albedo", _albedo);
		_shadowShader->setFloat("material.ka", _ka);
		_shadowShader->setVec3("material.kd", _kd);
		_shadowShader->setVec3("material.ks", _ks);
		_shadowShader->setFloat("material.ns", _ns);
		_shadowShader->setFloat("intensity", _intensity);

		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		
		//car
		glActiveTexture(GL_TEXTURE0);
		_blendTextures[1]->bind();
		_shadowShader->setMat4("model", _bunny->getModelMatrix());
		_bunny->draw();
		_blendTextures[1]->unbind();

		//rack track
		glActiveTexture(GL_TEXTURE0);
		_blendTextures[0]->bind();
		_shadowShader->setMat4("model", _race_track->getModelMatrix());
		_race_track->draw();
		_blendTextures[0]->unbind();

		//postures
		glActiveTexture(GL_TEXTURE0);
		_blendTextures[3]->bind();
		_shadowShader->setMat4("model", _postures[j]->getModelMatrix()*glm::scale(glm::mat4(1.0),glm::vec3(0.04,0.04,0.04)));
		_postures[j]->draw();
		_blendTextures[3]->unbind();
		
		//light ball	
		for (const auto& ball : hit_ball) {
			glActiveTexture(GL_TEXTURE0);
			_blendTextures[2]->bind();
			_shadowShader->setMat4("model", ball->getModelMatrix());
			ball->draw();
			_blendTextures[2]->unbind();
		}

		//unhitable ball	
		for (const auto& ball : unhit_ball) {
			glActiveTexture(GL_TEXTURE0);
			_blendTextures[2]->bind();
			_shadowShader->setMat4("model", ball->getModelMatrix());
			ball->draw();
			_blendTextures[2]->unbind();
		}

		//basic voxel
		_basic_voxel->draw(projection, view);

		//sky box
		_skyboxShader->use();
		glm::mat4 view1 = glm::mat4(glm::mat3(view));
		_skyboxShader->setMat4("projection", projection);
		_skyboxShader->setMat4("view", view1);
		_skyboxShader->setInt("cubemap", 0);
		_skybox->draw();		

		// draw ui elements
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		const auto flags =
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings;
		clock_t temptime;
		clock_t time = 0;
		clock_t ftime;
		if (_bunny->ready == 1 && _bunny->started == 0) temptime = clock() - _bunny->starttime;
		else temptime = 0;
		if (_bunny->ended == 0 && _bunny->started == 1) _bunny->endtime = clock();
		if (!ImGui::Begin("Control Panel", nullptr, flags)) {
			ImGui::End();
		}
		else {
			ImGui::Text("Light position");
			ImGui::Separator();
			ImGui::SliderFloat("x##2", &_light->position.x, -2000.f, 2000.f);
			ImGui::SliderFloat("y##2", &_light->position.y, -2000.f, 2000.f);
			ImGui::SliderFloat("z##2", &_light->position.z, -2000.f, 2000.f);
			ImGui::NewLine();

			ImGui::Text("Material");
			ImGui::Separator();
			ImGui::ColorEdit3("albedo", (float*)&_albedo);
			ImGui::SliderFloat("ka", &_ka, 0.0f, 1.0f);
			ImGui::ColorEdit3("kd", (float*)&_kd);
			ImGui::ColorEdit3("ks", (float*)&_ks);
			ImGui::SliderFloat("ns", &_ns, 0.0f, 50.0f);
			ImGui::SliderFloat("intensity", &_intensity, 0.0f, 3.0f);

			ImGui::Text("check points: %d/%d", _bunny->checkpoints, 9);
			if (temptime > 0 && temptime < 1000)ImGui::Text("READY: 3");
			else if (temptime >= 1000 && temptime < 2000)ImGui::Text("READY: 2");
			else if (temptime >= 2000 && temptime < 3000)ImGui::Text("READY: 1");
			else if (temptime >= 3000) {
				ImGui::Text("Go");
				_bunny->started = 1;
				_bunny->starttime = clock();
				_bunny->endtime = clock();
			}
			else if (_bunny->started == 1 && _bunny->ended == 0) {
				ImGui::Text("Go");
				time = _bunny->endtime - _bunny->starttime;
			}
			else {
				ImGui::Text("Finished");
				time = _bunny->endtime - _bunny->starttime;
			}
			ImGui::Text("time: %2d:%2d:%2d:%2d", time / 3600000, time % 3600000 / 60000, time % 60000 / 1000, time % 1000);
			
			ImGui::End();

		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
