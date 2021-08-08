#pragma once
#include<iostream>
#include "object3d.h"

class Camera : public Object3D {
public:
	float Yaw;
	glm::vec3 front;
	bool is_fixed;
	bool is_orbitting;

	glm::mat4 getViewMatrix() const;

	virtual glm::mat4 getProjectionMatrix() const = 0;

	void fix_camera(glm::vec3 position, glm::vec3 front);
};


class PerspectiveCamera : public Camera {
public:
	float fovy;
	float aspect;
	float znear;
	float zfar;

public:
	PerspectiveCamera(float fovy, float aspect, float znear, float zfar);
	
	~PerspectiveCamera() = default;
	glm::mat4 getProjectionMatrix(float _aspect) const;
	glm::mat4 getProjectionMatrix() const override;

	void zoomIn();
	void zoomOut();
	void zoomFit();
	void orbit(glm::vec3 aimed_position, int direction, float deltatime);
};


class OrthographicCamera : public Camera {
public:
	float left;
	float right;
	float bottom;
	float top;
	float znear;
	float zfar;
public:
	OrthographicCamera(float left, float right, float bottom, float top, float znear, float zfar);
	
	~OrthographicCamera() = default;

	glm::mat4 getProjectionMatrix() const override;
};