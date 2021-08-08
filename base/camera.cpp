#include "camera.h"

glm::mat4 Camera::getViewMatrix() const {
	return (is_fixed)? glm::lookAt(position, position + front, getUp()):glm::lookAt(position, position + getFront(), getUp());
}

void Camera::fix_camera(glm::vec3 position, glm::vec3 front)
{
	this->position = position;
	this->front = front;
	this->Yaw = glm::acos(front.x / front.length());
}


PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float znear, float zfar)
	: fovy(fovy), aspect(aspect), znear(znear), zfar(zfar) { }

glm::mat4 PerspectiveCamera::getProjectionMatrix() const {
	return glm::perspective(fovy, aspect, znear, zfar);
}

glm::mat4 PerspectiveCamera::getProjectionMatrix(float _aspect) const {
	return glm::perspective(fovy, _aspect, znear, zfar);
}

void PerspectiveCamera::zoomIn()
{
	if (fovy > glm::radians(30.0f)) {
		fovy -= glm::radians(1.0f);
	}
}

void PerspectiveCamera::zoomOut()
{
	if (fovy < glm::radians(70.0f)) {
		fovy += glm::radians(1.0f);
	}
}

void PerspectiveCamera::zoomFit()
{
	fovy = glm::radians(50.0f);
}

void PerspectiveCamera::orbit(glm::vec3 aimed_position, int direction, float deltatime)
{
	Yaw += direction * glm::radians(20.0f) * deltatime;
	glm::vec3 r = aimed_position - position;
	r = glm::vec3(-20.0f * std::cos(Yaw), 0.0 , -20.0f * std::sin(Yaw));
	
	position = aimed_position - r;
	front = glm::normalize(r);
}


OrthographicCamera::OrthographicCamera(
	float left, float right, float bottom, float top, float znear, float zfar)
	: left(left), right(right), top(top), bottom(bottom), znear(znear), zfar(zfar) { }

glm::mat4 OrthographicCamera::getProjectionMatrix() const {
	return glm::ortho(left, right, bottom, top, znear, zfar);
}